<?php

/**
 * @file
 * Emma module admin settings.
 */

/**
 * Return the Emma global settings form.
 */
function emma_admin_settings() {
  $form['emma_api_key'] = array(
      '#type' => 'textfield',
      '#title' => t('Account ID'),
      '#required' => TRUE,
      '#default_value' => variable_get('emma_api_key', ''),
      '#description' => t('The API key for your Emma account. Get a valid API key at your !apilink.', array('!apilink' => l(t('Emma API Dashboard'), 'https://app.e2ma.net/app2/billing/settings/')))
  );

  $form['emma_username'] = array(
      '#type' => 'textfield',
      '#title' => t('Public API Key'),
      '#required' => TRUE,
      '#default_value' => variable_get('emma_username', ''),
      '#description' => t('Username/Public Key for your Emma API. Generate Emma Username at your !apilink.', array('!apilink' => l(t('Emma API Dashboard'), 'https://app.e2ma.net/app2/billing/settings/')))
  );

  $form['emma_password'] = array(
      '#type' => 'password',
      '#title' => t('Private API Key'),
      '#required' => TRUE,
      '#default_value' => variable_get('emma_password', ''),
      '#description' => t('Password/Private Key for your Emma API. Generate Emma Password at your !apilink.', array('!apilink' => l(t('Emma API Dashboard'), 'https://app.e2ma.net/app2/billing/settings/')))
  );  
  
  return system_settings_form($form);
}
