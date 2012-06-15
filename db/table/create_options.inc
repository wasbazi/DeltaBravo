<?php

class DB_CreateOptions extends GC_DatabaseConnection {

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
    foreach(){
      $column['name'] = strtolower($column['name']);
      $name_format = preg_match('/^[a-z][a-z0-9]*$/',$column['name']);
    }
    return $col;
	}
  // TODO: 
  // 1) Foreach loop to run through $options has arrays with, $column['name'], 
  //    and then options like $column['type'], $column['length']
  // 2) Make sure col() checks syntax
  // 3) Test col_to_string
  // 4) Change $this->actions??
  private function col_to_string($data, $required, $optional){
    $column = "";
    foreach($required as $component){
      if(isset($data[$component])){
        $column .= ' ' . $data[$component] . ' ';
      }else{
        return false;
      }
    }
    foreach($optional as $component){
      if(isset($data[$component])){
        $column .= ' ' . $data[$component] . ' ';
      }
    }
    return $column;
  }
}