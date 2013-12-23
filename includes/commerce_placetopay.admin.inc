<?php

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

function mensaje_transaccion_pendiente_admin_settings_form($form, &$form_state) {
    $form = array();
    
    $form['mensaje_transaccion_pendiente'] = array(
        '#type' => 'textarea',
        '#title' => t('Pending transaction message'),
        '#description' => t('Configuring the pending transaction message.'),
        '#default_value' => variable_get('mensaje_transaccion_pendiente', ''),
        '#required' => TRUE,
    );
    
    return system_settings_form($form);
}