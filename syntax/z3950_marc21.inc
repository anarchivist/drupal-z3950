<?php
// $Id$

/**
 * Implement z39.50 syntax for Marc21
 */
function z3950_marc21($op, $id = NULL, $pos = NULL) {
  switch ($op) {
    case 'info':
      return array(
        'options' => array('1.2.840.10003.5.10' => 'Marc21'),
        'value' => 'USmarc', // returned by yaz_record
      );

    case 'summary':
      return _z3950_marc21_summary($id, $pos);

    case 'detail':
      return _z3950_marc21_detail($id, $pos);
  }
}

function _z3950_marc21_summary($id, $pos) {
  $marc_array = _z3950_marc21_record($id, $pos);

  // start the summary return array
  $summary = array('type' => t('metadata'), 'extra' => array());

  // add title
  if (isset($marc_array['3,245'])) {
    $summary['title'] = implode(' ', array_pop($marc_array['3,245']));
  }

  // add snippet
  $snippet = array();
  foreach (array('3,100', '3,260', '3,300') as $key) {
    if (isset($marc_array[$key])) {
      $snippet[] = implode(' ', array_pop($marc_array[$key]));
    }
  }
  $summary['snippet'] = implode(' ', $snippet);

  return $summary;
}

function _z3950_marc21_detail($id, $pos) {
  // set page title
  $marc_array = _z3950_marc21_record($id, $pos);

  // set the title
  if (isset($marc_array['3,245'])) {
    drupal_set_title(implode(' ', array_pop($marc_array['3,245'])));
  }

  // return page text
  // @TODO: format a better page for display
  return yaz_record($id, $pos, 'string');
}

function _z3950_marc21_record($id, $pos) {
  $marc_array = array();
  foreach (yaz_record($id, $pos, 'array') as $yaz_element) {
    if (isset($yaz_element[1])) {
      // strip all non-essential chars from the key, to insure a safe eval() below
      $key = preg_replace('/[^0-9a-z,@\(\)]/', '', $yaz_element[0]);

      // convert the parenthesis's into array brackets, and quote the array elements
      $key = str_replace(array('(', ')'), array("['", "']"), $key);

      // store the value in the multidimensional array
      eval('$marc_array'. $key .' = $yaz_element[1];');
    }
  }
  return $marc_array;
}
