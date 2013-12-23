<?php
function commerce_placetopay_response($order) {
  module_load_include('inc', 'commerce_placetopay', 'includes/PlacetoPay');

  $content = array();

  $PaymentResponse = (isset($_POST['PaymentResponse']) ? $_POST['PaymentResponse']: FALSE);

  $instance_id = $order->data['payment_method'];

  $payment_method = commerce_payment_method_instance_load($instance_id);

  $settings = $payment_method['settings'];

  $p2p = new PlacetoPay();

  $p2p->setGPGProgramPath($settings['gnupg_program_path']);//GNUPG_PROGRAM_PATH

  $p2p->setGPGHomeDirectory($settings['gnupg_home_directory']);//GNUPG_HOME_DIRECTORY

  $rc = $p2p->getPaymentResponse($settings['p2p_key_id'], $settings['p2p_passphrase'], $PaymentResponse);

  $rows = array(
    array('Fecha y Hora:', $p2p->getTransactionDate()),
    array('Estado:'),
    array('Motivo:', $p2p->getErrorCode() . ' - ' . $p2p->getErrorMessage()),
    array('Valor:', $p2p->getCurrency() + $p2p->getTotalAmount()),
    array('IVA:', $p2p->getCurrency() + $p2p->getTaxAmount()),
    array('Franquicia:', $p2p->getFranchiseName()),
    array('Banco:', $p2p->getBankName()),
    array('Autorizaci—n/CUS:', $p2p->getAuthorization()),
    array('Recibo:', $p2p->getReceipt()),
    array('Referencia:', $p2p->getReference()),
    array('Descripci—n:', 'Pediente'),
    array('Direcci—n:', ip_address()),
    array('Cliente:', $p2p->getShopperName()),
    array('Email:', $p2p->getShopperEmail()),
  );

  switch($rc) {
    case PlacetoPay::P2P_ERROR:
    case PlacetoPay::P2P_DECLINED:
      $rows[1][] = 'Transacci—n rechazada';
      break;

    case PlacetoPay::P2P_APPROVED:
    case PlacetoPay::P2P_DUPLICATE:
      $rows[1][] = 'Transacci—n Aprobada';
      break;

    case PlacetoPay::P2P_PENDING:
      $rows[1][] = 'Transacci—n Pendiente';
      break;
  }

  $rows[][] = array(
    'data' => 'Si tiene alguna inquietud conta?ctenos al Tele?fono (XX)(X) XXX XXXX o vi?a email mispagos@micomercio.com',
    'colspan' => 2,
  );

  $content['#markup'] = theme('table', array('rows' => $rows));

  return $content;
}