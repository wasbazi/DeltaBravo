<?php

class DB_QueryOperations extends DatabaseConnection {

  public function __construct(){
    require_once("query" . DIRECTORY_SEPARATOR . "condition.inc");
    require_once("query" . DIRECTORY_SEPARATOR . "modifier.inc");
    require_once("query" . DIRECTORY_SEPARATOR . "join.inc");
  }

  public function select($table, $column = "*"){
    $this->operation = "SELECT ";
    if ($column == "*") {
      $this->operation .= $column;
    }
    elseif (is_scalar($column)) {
      $this->operation .= "`$column`";
    }
    else {
      $this->operation .= '`' . implode('`, `', $column) . '`';
    }
    $this->operation .= " FROM $table";
    $this->pdo = 'query';
    return $this;
  }
  
  public function update($table, $data){
    $update = "";
    $columns = array_keys($data);
    for($i = 0; $i<count($columns); $i++){
      $key = $columns[$i];
      $update .= ($i > 0) ? ',' : '';
      $update .= "`$key` = ? ";
    }
    
    $this->operation = "UPDATE `$table` SET " . $update;
    
    // Store the values for the statement for use in execute() to be used in place
    // of the question marks in $this->operation
    $this->placeholders = array_values($data);
    
    return $this;
  }
  
  public function delete($table){
  
    $this->operation = "DELETE FROM $table";

    // Store the type of statement as a query for different handling in execute()
    // 'count' will specify that the result be returned after calling PDO's
    // rowCount() function - A number representing the rows affected
    $this->pdo = "count";
    
    return $this;
  }
  
  public function insert($table, $data){
    $columns = array_keys($data);
    $columns_string = implode("`, `", $columns);
    
    $placeholder_string = "?";
    
    if(count($columns > 1)){
      for($i = 1; $i < count($columns); $i++){
        $placeholder_string .= " , ?";
      }
    }

    $this->operation = "INSERT INTO `$table` (`$columns_string`) VALUES ($placeholder_string)";
    $this->placeholders = array_values($data);
    $this->pdo = 'count';
    
    // Return $this so the next class will be applied to DB_QueryOperations
    return $this;
  }

  public function desc($table) {
    $this->operation = "DESC `$table`";
    $this->pdo = 'query';
    return $this;
  }
  
  public function execute($debug = false, $json = false){
    $query_parts = array('operation', 'join', 'condition', 'modifier');
    $query = "";
    foreach($query_parts as $key => $part){
      if(isset($this->$part)){
        $query .= $this->$part . " ";
      }
    }
    
    // Set Placeholders for counter SQL-Injection Measures
    $placeholders = isset($this->placeholders) ? $this->placeholders : array();
    $pdo = (isset($this->pdo)) ? $this->pdo : '';
    return parent::execute($query, $placeholders, $pdo, $debug, $json);
  }
  
  public function __call($name, $arguments){
  
    $condition_methods = get_class_methods('DB_Condition');
  
    if(in_array($name, $condition_methods)){
      $condition = new DB_Condition();
      $clause = call_user_func_array(array($condition, $name), $arguments);
      $this->condition = $clause->where;
      if(isset($clause->placeholders)){
        if(isset($this->placeholders)){
          $this->placeholders = array_merge($this->placeholders,$clause->placeholders);
        }else{
          $this->placeholders = $clause->placeholders;
        }
      }
    }
    
    $join_methods = get_class_methods('DB_Join');
    
    if(in_array($name, $join_methods)){
      $join = new DB_Join();
      $this->join = call_user_func_array(array($join, $name), $arguments);
    }
    
    $modifier_methods = get_class_methods('DB_Modifier');
   
    if(in_array($name, $modifier_methods)){
      $modifier = new DB_Modifier();
      $this->modifier = call_user_func_array(array($modifier, $name), $arguments);
    }
    
    // Return $this so the next class will be applied to DB_QueryOperations
    return $this;
  }
}
