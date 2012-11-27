<?php

/**
 * Example use of DatabaseConnection:
 * $gc = new DatabaseConnection('localhost', 'database', 'username', 'password');
 * $gc->select(table, column)->where(key, value, conditional)->sort(column)->execute();
 *
 * More examples can be found in example.php
 */

class DatabaseConnection {

  private static $hostname;
  private static $database;
  private static $username;
  private static $password;
  private static $type;
  private static $encoding;

  protected $operations_methods;

  public function __construct($hostname, $database, $username, $password, $type = 'mysql', $encoding = 'utf8') {
    require_once("db" . DIRECTORY_SEPARATOR . "query_operations.inc");
    require_once("db" . DIRECTORY_SEPARATOR . "table_operations.inc");

    self::$hostname = $hostname;
    self::$database = $database;
    self::$username = $username;
    self::$password = $password;
    self::$type = $type;
    self::$encoding = $encoding;

    $this->operations_methods = array(
      array( 
        'methods' => get_class_methods('DB_TableOperations'), 
        'class' => 'DB_TableOperations', 
        'instance' => FALSE 
        ),
      array( 
        'methods' => get_class_methods('DB_QueryOperations'), 
        'class' => 'DB_QueryOperations', 
        'instance' => FALSE 
        )
    );
  }

  public function connect(){
    try {
      switch(strtolower(self::$type)) {
        case "mysql":
          $connection_options = 'mysql:host=' . self::$hostname . ';dbname=' . self::$database . ';charset=' . self::$encoding;
          $db = new PDO($connection_options, self::$username, self::$password, array(PDO::ATTR_PERSISTENT => true));
          return $db;
        default:
          throw new Exception("Driver not yet implemented", 1);
      }
    }catch(PDOException $e) { // Catch PDOException and echo error
      echo $e->getMessage();
    }
  }

  public function execute($query, $placeholders, $pdo, $debug = false, $json = false){
    if(!isset($pdo)){
      return false;
    }

    if($debug){
      $debug_info = $this->debug_query($query, $placeholders, $json);
      if(FALSE !== $debug_info){
        return $debug_info;
      }
    }

    try {      
      $result = $this->run_query($query, $placeholders, $pdo);
    }catch(PDOException $e){
      echo $e->getMessage();
    }   

    return $result;
  }

  private function run_query($query, $placeholders, $pdo){
    $connection = self::connect();

    if($pdo == 'exec'){
        $result = $connection->exec($query);
    }else if($pdo == 'count'){
        $result = $statement->rowCount();
    }else{
        $statement = $connection->prepare($query);
        $result = $statement->execute($placeholders);        
        $result = $statement->fetchAll(PDO::FETCH_ASSOC);
        
        if($pdo == 'list_tables' && $result !== FALSE){            
          foreach ($result as $k => $v) {
            reset($result[$k]);
            $result[$k] = current($result[$k]);
          }
        }
    }
    
    $this->close($connection);
    return empty($result) ? FALSE : $result;
  }
    
  private function debug_query($query, $placeholders, $json){
    if ($json) {
      $json = array('query' => $query, 'placeholders' => $placeholders);
      return $json;
    }

    echo sprintf("<pre>Query: '%s'</pre><br />", print_r($query,1));
    echo sprintf("<pre>Placeholders: %s</pre><br />", print_r($placeholders,1));
    return false;
  }

  public function last_insert_id(){
    $db = product.html;
    $connection = $this->connection;
    $last_insert = $db->lastInsertId();
    $this->close($connection);
    return $last_insert;
  }
    
  public function close(&$connection){
    $connection = null;
  }
  
  /***************************
   * Used to catch unspecified functions, 
   * all query handling is done through __call()
   *
   * The operations classes are lazy loaded 
   **************************/
  public function __call($name, $arguments){
    foreach($this->operations_methods as $class_info){
      if(in_array($name, $class_info['methods'])){
        if($class_info['instance'] === FALSE){
          $class_info['instance'] = new $class_info['class']();
        }
        return call_user_func_array(array($class_info['instance'], $name), $arguments);
      }
    }
    return false;
  }
}
