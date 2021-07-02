<?php

$plugin = array(
  'title' => 'Legal Document',
  'description' => 'Output legal document for user to agree or check (if agreed).',
  'single' => TRUE,
  'render callback' => 'legal_document_content_type_render',
  'edit form' => 'legal_document_content_type_edit_form',
  'admin title' => 'legal_document_admin_title',
  'category' => 'Gang',
  'all contexts' => TRUE,
);

/**
 * Ctools edit form.
 */
function legal_document_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  $documents = entity_legal_get_all_documents();
  foreach ($documents as $document) {
    $options[$document->name] = $document->label(TRUE);
  }

  $form['document'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#title' => 'Legal document',
    '#default_value' => !empty($conf['document']) ? $conf['document'] : '',
  );
  $form['agree'] = array(
    '#type' => 'checkbox',
    '#title' => 'Use agree form',
    '#default_value' => !empty($conf['agree']) ? $conf['agree'] : TRUE,
  );
  $form['admin_title'] = array(
    '#type' => 'textfield',
    '#title' => 'Administrative title',
    '#default_value' => $conf['admin_title'],
    '#weight' => -1000,
  );
  return $form;
}

/**
 * Ctools edit form submit handler.
 */
function legal_document_content_type_edit_form_submit($form, &$form_state) {
  $form_state['conf']['document'] = $form_state['values']['document'];
  $form_state['conf']['agree'] = $form_state['values']['agree'];
  $form_state['conf']['admin_title'] = $form_state['values']['admin_title'];
}

/**
 * Implements hook_admin_title().
 */
function legal_document_admin_title($subtype, $conf, $context = NULL) {
  $title = empty($conf['admin_title']) ? '' : ': ' . $conf['admin_title'];
  return t('Legal' . $title);
}

/**
 * Render callback function.
 */
function legal_document_content_type_render($subtype, $conf, $args, $contexts) {
  $legal = entity_load_single(ENTITY_LEGAL_DOCUMENT_ENTITY_NAME, $conf['document']);
  $legal->manual_require = $conf['agree'];
  $output = render(entity_view(ENTITY_LEGAL_DOCUMENT_ENTITY_NAME, array($legal)));
  $block = new stdClass();

  $block->content = $output;
  return $block;
}
