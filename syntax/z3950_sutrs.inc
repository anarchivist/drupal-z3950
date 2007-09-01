<?php
// $Id$

/**
 * Implement z39.50 syntax for SUTRS
 */
function z3950_sutrs($op, $id = NULL, $pos = NULL) {
  switch ($op) {
    case 'info':
      return array(
        'options' => array('1.2.840.10003.5.101' => 'SUTRS'),
        'value' => 'SUTRS', // returned by yaz_record
      );

    case 'summary':
      return _z3950_sutrs_summary($id, $pos);

    case 'detail':
      return _z3950_sutrs_detail($id, $pos);
  }
}

function _z3950_sutrs_summary($id, $pos) {
  $rec_string = yaz_record($id, $pos, 'string');
  if (!empty($rec_string)) {
    return array(
      'type' => t('metadata'),
      'title' => $rec_string,
    );
  }
}

function _z3950_sutrs_detail($id, $pos) {
  // set page title
  drupal_set_title(t('Metadata Contents'));

  // get page text
  return yaz_record($id, $pos, 'string');
}
