<?php

class DB_Modifier {
  public function group($column, $format = ''){
    return "GROUP BY $column $format";
  }
  
  public function order($column, $format = 'asc'){
  	if (is_array($column)) {
  		foreach($column as &$c) {
  			$c = implode(' ', $c);
  		}
  		$column = implode(', ', $column);
  		return "ORDER BY $column";
  	}
  	else {
    	return "ORDER BY $column $format";
    }
  }
  
  public function limit($limit_number, $offset = '0'){
    return "LIMIT $offset, $limit_number";
  }
}