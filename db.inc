<?php

/**
 * Example use of DatabaseConnection:
 * $gc = new DatabaseConnection('localhost', 'database', 'username', 'password');
 * $gc->select(table, column)->where(key, value, conditional)->sort(column)->execute();
 */

class DatabaseConnection {

  private static $hostname;
  private static $database;
  private static $username;
  private static $password;
  private static $type;
  private static $encoding;

  public function __construct($hostname, $database, $username, $password, $type = 'mysql', $encoding = 'utf8') {
    require_once("db" . DIRECTORY_SEPARATOR . "query_operations.inc");
    require_once("db" . DIRECTORY_SEPARATOR . "table_operations.inc");

    self::$hostname = $hostname;
    self::$database = $database;
    self::$username = $username;
    self::$password = $password;
    self::$type = $type;
    self::$encoding = $encoding;
  }

  // Function to connect to the database, Object $newSettings 
  public function connect(){
    try { // Try to use PDO to connect
      switch(strtolower(self::$type)) { // Sanitize database type to lowercase
        case "mysql": // Setup MySQL PDO connection
          $db = new PDO('mysql:host=' . self::$hostname . ';dbname=' . self::$database . ';charset=' . self::$encoding, self::$username, self::$password, array(PDO::ATTR_PERSISTENT => true));
          return $db;
          break;
        case "couch": // Setup CouchDB PDO connection using custom driver
          throw new Exception("Couch DB driver not yet implemented", 1);
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
      if ($debug && $json) {
        $json = array('query' => $query, 'placeholders' => $placeholders);
        return $json;
      }
      elseif ($debug) {
        echo sprintf("<pre>Query: '%s'</pre><br />", print_r($query,1));
        if(!empty($placeholders)){
          echo sprintf("<pre>Placeholders: %s</pre><br />", print_r($placeholders,1));
        }
      }
      
      if (isset($pdo) && $pdo == 'exec') {
        $result = $connection->exec($query);
      }
      else {
        $statement = $connection->prepare($query);
        $result = $statement->execute($placeholders);
        if (isset($pdo) && $pdo == 'query') {
          $result = $statement->fetchAll(PDO::FETCH_ASSOC);
          if (empty($result)) {
            $result = false;
          }
        }
        elseif (isset($pdo) && $pdo == 'count') {
          $result = $statement->rowCount();
        }
        elseif (isset($pdo) && $pdo == 'list_tables') {
          $result = $statement->fetchAll(PDO::FETCH_ASSOC);
          if(empty($result)) {
            $result = FALSE;
          }
          else {
            foreach ($result as $k => $v) {
              reset($result[$k]);
              $result[$k] = current($result[$k]);
            }
          }
        }
        elseif (isset($pdo) && $pdo == 'list_columns') {
          $result = $statement->fetchAll(PDO::FETCH_ASSOC);
          if(empty($result)) {
            $result = FALSE;
          }
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

  // Get the ID using PDO's lastInsertId
  public function last_insert_id(){
    $db = new PDO('mysql:host=' . self::$hostname . ';dbname=' . self::$database . ';charset=' . self::$encoding, self::$username, self::$password, array(PDO::ATTR_PERSISTENT => true));
    $connection = $this->connection;
    $last_insert = $db->lastInsertId();
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
    $table_operations_methods = get_class_methods('DB_TableOperations');
    if(in_array($name, $table_operations_methods)){ 
      $table_operations = new DB_TableOperations();
      
      // Calls the function in the scope of $table_operations, the relevant function is
      // stored in $name, with $arguments passed for the functions arguments
      return call_user_func_array(array($table_operations, $name), $arguments);
    }

    $query_operations_methods = get_class_methods('DB_QueryOperations');
    if(in_array($name, $query_operations_methods)){
      $query_operations = new DB_QueryOperations();
      // Calls the function in the scope of $query_operations, the relevant function is
      // stored in $name, with $arguments passed for the functions arguments
      return call_user_func_array(array($query_operations, $name), $arguments);
    }
  }
}
