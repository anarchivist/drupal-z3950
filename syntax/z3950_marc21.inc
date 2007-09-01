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

    case 'details':
      return _z3950_marc21_details($id, $pos);
  }
}

function _z3950_marc21_summary($id, $pos) {
  $rec_array = yaz_record($id, $pos, 'array');
  if (is_null($rec_array)) {
    $rec_string = yaz_record($id, $pos, 'string');
    if (!empty($rec_string)) {
      return array(
        'type' => t('metadata'),
        'title' => $rec_string,
      );
    }
  }
  else {
    return array(
      'type' => t('metadata'),
      'link' => url('z3950/'. $server_index .'/'. $pos .'/'. $keys),
      'title' => $rec_array[4][1],
      'snippet' => $rec_array[10][1],
      'extra' => array(
        'time_period' => $rec_array[13][1],
      ),
    );
  }
}

function _z3950_marc21_details($id, $pos) {
  // set page title
  $rec_array = yaz_record($id, $pos, 'array');
  if (!is_null($rec_array)) {
    drupal_set_title($rec_array[4][1]);
  }

  // return page text
  return yaz_record($id, $pos, 'string');
}
