<?php

class GC_DB_TableOperations extends GC_DatabaseConnection{

  public function __construct(){
    require_once("create_options.inc");
    require_once("alter_options.inc");
  }

  public function create($name, $exists = false){
    $this->action = 'CREATE TABLE ';
    $this->action .= ($exists) ? 'IF EXISTS ' : 'IF NOT EXISTS ';
    $this->action .= $name;
    
    $this->col_class = new GC_DB_CreateOptions;
    $this->pdo = 'exec';
    return $this;
  }
  public function alter($name){
    $this->action = 'ALTER TABLE ' . $name;
    
    $this->col_class = new GC_DB_AlterOptions;
    return $this;
  }

  public function columns($table){
    $this->action = "DESCRIBE $table";
    //$this->placeholders = array($table);
    $this->pdo = 'query';
    
    return $this;
  }
  
  public function execute($debug = false){
    if(isset($this->action)){
      $query = $this->action;
    }else{
      return false;
    }
    $query .= (isset($this->col)) ? ' ' . $this->col : '';
    $pdo = (isset($this->pdo)) ? $this->pdo : '';
    $placeholders = (isset($this->placeholders)) ? $this->placeholders : array();
    return parent::execute($query, $placeholders, $pdo, $debug);
  }
  
  public function __call($name, $arguments){
    $this->col = call_user_func_array(array($this->col_class,$name), $arguments);
    return $this;
  }
}

