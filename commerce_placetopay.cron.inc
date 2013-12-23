<?php
function commerce_placetopay_run_cron() {
	$result = db_select('commerce_placetopay_pending', 'p')
		->fields('p')
		->condition('cppay_state', 1)
		->range(0, 10)
		->execute()
		->fetchAll();

	if ($result) {
		module_load_include('inc', 'commerce_placetopay', 'includes/PlacetoPay');
		$p2p = new PlacetoPay();

		foreach($result as $item) {
			$reference = get_referencia_placetopay($item->order_id);
			$rc = $p2p->queryPayment('{D7357208-4EE9-11E3-AC6F-003005CE861E}', $reference, $item->cppay_currency, $item->cppay_amount);
			if ((($rc == PlacetoPay::P2P_ERROR) && ($p2p->getErrorCode() != 'HTTP')) || ($rc == PlacetoPay::P2P_DECLINED)) {
				// actualice la BD, no se hizo el pago
				db_update('commerce_placetopay_pending')
					->fields(array(
						'cppay_state' => 0,
					))
					->condition('cppay_id', $item->cppay_id)
					->execute();
			}
			elseif (($rc == PlacetoPay::P2P_APPROVED) || ($rc == PlacetoPay::P2P_DUPLICATE)) {
				// actualice la BD, asiente el pago
				db_update('commerce_placetopay_pending')
					->fields(array(
						'cppay_state' => 0,
					))
					->condition('cppay_id', $item->cppay_id)
					->execute();

				$order = commerce_order_load($item->order_id);

				$transaction = commerce_payment_transaction_new('commerce_placetopay', $order->order_id);
				$transaction->remote_id = $reference;
				$transaction->instance_id = $item->cppay_method;
				$transaction->amount = $order->commerce_order_total[LANGUAGE_NONE][0]['amount'];
				$transaction->currency_code = $order->commerce_order_total[LANGUAGE_NONE][0]['currency_code'];
				$transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
				commerce_payment_transaction_save($transaction);

				commerce_payment_redirect_pane_next_page($order);
			}
			elseif ($rc == PlacetoPay::P2P_PENDING) {
				// no haga nada
			}
			else {
				// genere un log, pudo ser un problema de telecomunicaciones
			}
		}
	}

	return 1;
}