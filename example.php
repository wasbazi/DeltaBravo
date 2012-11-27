<?php

header('Content-type: text/plain');

include('db.inc');

$db = new DatabaseConnection('localhost', 'dbtest', 'dbtest', 'dbtest', 'mysql', 'utf8');

$where = array(
	array( 'fid', 1, '>', 'AND' ),
	array( 'fruit', 'mango', '<' ),
	);
$order = array(
	array('fruit', 'DESC'),
	array('id', 'DESC'),
	);

$results['select'] = $db->select('fruits')->execute();
$results['select/where'] = $db->select('fruits')->where('fid', 1)->execute();
$results['select/where/op'] = $db->select('fruits')->where('fruit', 'a%', 'LIKE')->execute();
$results['select/where/multi'] = $db->select('fruits')->where($where)->execute();
$results['select/join/using'] = $db->select('fruits')->join('test', 'fid')->execute();
$results['select/join/on'] = $db->select('fruits')->join('test', array('fruits.fid', 'test.fid', '='))->execute();
$results['select/join/on/multi'] = $db->select('fruits')->join('animal', array(array('animals.id', 'test.id', '='),array('animals.animals', 'test.fid', '<')))->execute();
$results['select/join/using/where'] = $db->select('fruits')->join('test', 'id')->where('id', 1)->execute();
$results['select/where/join/using'] = $db->select('fruits')->where('id', 2)->join('test', 'id')->execute();
$results['select/order'] = $db->select('fruits')->order('id', 'DESC')->execute();
$results['select/order/multi'] = $db->select('fruits')->order($order)->execute();
$results['select/limit'] = $db->select('fruits')->limit(2)->execute();
$results['select/limit/offset'] = $db->select('fruits')->limit(2, 2)->execute();

$results['desc'] = $db->desc('fruits')->execute();

foreach ($results as &$r) {
	if (!$r) {
		if (ob_start()) {
			var_dump($r);
			$r = ob_get_contents();
			ob_end_clean();
		}
	}
}
print_r($results);