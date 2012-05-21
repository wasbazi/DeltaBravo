<?php

include('../db.inc');

$db = new DatabaseConnection('localhost', 'dbtest', 'dbtest', 'dbtest', 'mysql', 'utf8');

$where = array(
	array( 'id', 1, '>', 'AND' ),
	array( 'content', 'zebra', '<' ),
	);
$order = array(
	array('content', 'DESC'),
	array('id', 'DESC'),
	);

$results['select'] = $db->select('test1')->execute();
$results['select/where'] = $db->select('test1')->where('id', 1)->execute();
$results['select/where/op'] = $db->select('test1')->where('content', 'a%', 'LIKE')->execute();
$results['select/where/multi'] = $db->select('test1')->where($where)->execute();
$results['select/join/using'] = $db->select('test1')->join('test2', 'id')->execute();
$results['select/join/on'] = $db->select('test1')->join('test2', array('test1.id', 'test2.id', '='))->execute();
$results['select/join/on/multi'] = $db->select('test1')->join('test2', array(array('test1.id', 'test2.id', '='),array('test1.content', 'test2.animal', '>')))->execute();
$results['select/join/using/where'] = $db->select('test1')->join('test2', 'id')->where('id', 1)->execute();
$results['select/where/join/using'] = $db->select('test1')->where('id', 2)->join('test2', 'id')->execute();
$results['select/order'] = $db->select('test1')->order('id', 'DESC')->execute();
$results['select/order/multi'] = $db->select('test1')->order($order)->execute();
$results['select/limit'] = $db->select('test1')->limit(2)->execute();
$results['select/limit/offset'] = $db->select('test1')->limit(2, 2)->execute();

$results['desc'] = $db->desc('test1')->execute();

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