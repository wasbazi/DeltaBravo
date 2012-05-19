<?php

/*
 *
 * Example use of GC_DatabaseConnection:
 * $gc = new GC_DatabaseConnection();
 * $gc->select(table, column)->where(key, value, conditional)->sort(column)->execute();
 *
 */
 

 
class GC_DatabaseConnection extends GreenCoffee{

  public function __construct(){
    require_once("db" . DIRECTORY_SEPARATOR . "query_operations.inc");
    require_once("db" . DIRECTORY_SEPARATOR . "table_operations.inc");
  }

  // Function to connect to the database, Object $newSettings 
  public function connect( $newSettings = 0 ){
    global $coffee_settings;

    if($newSettings == 0){ // If default settings, load from settings object
      $connection_options = $coffee_settings->db_settings; 
    } else{ // If new settings, use input
      $connection_options = $newSettings;
    }
    
    // Declare all connection option variables by using variable variables
    foreach( $connection_options as $key => $value ){
      ${$key} = $value;
    }
    
    try { // Try to use PDO to connect
      switch(strtolower($type)) { // Sanitize database type to lowercase
        case "mysql": // Setup MySQL PDO connection
          $db = new PDO("mysql:host=$hostname;dbname=$database", $username, $password, array(PDO::ATTR_PERSISTENT => true));
          return $db;
          break;
        case "couch": // Setup CouchDB PDO connection using custom driver
          
          break;
      }
    }catch(PDOException $e) { // Catch PDOException and echo error
      echo $e->getMessage();
    }
  }
  public function execute($query, $placeholders, $pdo, $debug = false, $json = false){
    try {
      // Connect to the database
      $connection = $this->connect();
      
      // Query string parsed
      if($debug && $json){
        $json = array('query' => $query, 'placeholders' => $placeholders);
        return $json;
      }elseif($debug){
        echo sprintf("<pre>Query: '%s'</pre><br />", print_r($query,1));
        if(!empty($placeholders)){
          echo sprintf("<pre>Placeholders: %s</pre><br />", print_r($placeholders,1));
        }
      }
      
      if(isset($pdo) && $pdo == 'exec'){
        $result = $connection->exec($query);
      }else{
        $statement = $connection->prepare($query);
        $result = $statement->execute($placeholders);
        if(isset($pdo) && $pdo == 'query'){
          $result = $statement->fetchAll(PDO::FETCH_ASSOC);
          if(empty($result)) {
            $result = false;
          }
        }else if(isset($pdo) && $pdo == 'count'){
          $result = $statement->rowCount();
        }
        
      }
      // Close the connection
      $this->close($connection);
      // Return the result to be handled by the programmer
      return $result;
    }
    catch(PDOException $e){ // If there is an error, catches it and prints it.
      echo $e->getMessage();
    }
  }
  public function last_insert_id(){
    $connection = $this->connection;
    $last_insert = PDO::lastInsertId();
    $this->close($connection);
    return $last_insert;
  }
  
  // Function to close the database connection
  public function close(&$connection){
    $connection = null;
  }
  
  // Used to catch unspecified functions, all query handling 
  // is done through __call()
  public function __call($name, $arguments){
    
    $table_operations_methods = get_class_methods('GC_DB_TableOperations');
    if(in_array($name, $table_operations_methods)){ 
      $table_operations = new GC_DB_TableOperations();
      
      // Calls the function in the scope of $table_operations, the relevant function is
      // stored in $name, with $arguments passed for the functions arguments
      return call_user_func_array(array($table_operations, $name), $arguments);
    }

    $query_operations_methods = get_class_methods('GC_DB_QueryOperations');
    if(in_array($name, $query_operations_methods)){
      $query_operations = new GC_DB_QueryOperations();
      // Calls the function in the scope of $query_operations, the relevant function is
      // stored in $name, with $arguments passed for the functions arguments
      return call_user_func_array(array($query_operations, $name), $arguments);
    }
  }
}
