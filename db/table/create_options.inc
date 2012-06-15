<?php

class CreateOptions extends GreenCoffee{

  protected $actions = array(
    'BIT', 'TINYINT', 'SMALLINT', 'MEDIUMINT', 'INT', 'INTEGER', 'BIGINT', 
    'REAL', 'DOUBLE', 'FLOAT', 'DECIMAL', 'NUMERIC', 'DATE', 'TIME', 
    'DATETIME', 'YEAR', 'CHAR', 'VARCHAR', 'BINARY', 'VARBINARY', 'TINYBLOB',
    'BLOB', 'MEDIUMBLOB', 'LONGBLOB', 'TINYTEXT', 'TEXT', 'MEDIUMTEXT',
    'LONGTEXT', 'ENUM', 'SET',
  );

  // Array with position 0 in $actions array
  // position 1 are options
	public function col($options){
		foreach($options as $column){
      if(in_array($column, $actions)){
        $this->col .= (isset($this->col) ? ' ,' : '';
        $this->col .= implode(' ', $actions);
      }else{
        continue;
      }
    }
	}

}