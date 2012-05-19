<?php

class GC_DB_Join{
  /* 
     $table = string OR array( 'table1', 'table2', ... , 'tableN' )
     $on = array( 
      [0] => table1.column,
      [1] => table2.column
     )
     CONSTRUCT -> ON table1.column = table2.column */
  public function join($table, $on){
    $join = '';
    if(is_array($table)){
      foreach($table as $single){
        $join .= " JOIN $single ";
      }
    }else{
      $join .= "JOIN $table ";
    }
    
    foreach($on as $clause){
      if(isset($on_clause)){
        $on_clase .= ' AND ';
      }else{
        $on_clase = '';
      }
      $on_clause .= $on[0] . ' = ' . $on[1];
    }
    if(isset($on_clase)){
      $on_clause .= '(' . $on_clause . ')';
      $join .= $on_clause;
    }
    return $join;
  
  }

}

/* Todo:
 * Add functionality for different types of Join Clauses
 */
 