<?php

class DB_Join{
  /* 
     $table = string OR array(array($table, $op), array($table, $op), ...)
     $on = array( 
      [0] => table1.column,
      [1] => table2.column
     )
     OR
     $on = array(
      array(table1.column, table2.column)
     )
     OR
     $on = column_name

     CONSTRUCT -> ON table1.column = table2.column
     OR
     CONSTRUCT -> USING (column)
  */
   public function left_join($table, $op){
    $left = 'LEFT ';
    $join = self::join($table, $op);
    $left .= $join;
    return $left;
  }
  public function right_join($table, $op){
    $right = 'RIGHT ';
    $join = self::join($table, $op);
    $right .= $join;
    return $right;
  }
  
  public function join($table, $op){
    $join = '';
    if(is_scalar($table)){
      $join .= "JOIN `$table`";
      if (is_scalar($op)) {
        $join .= " USING (`$op`)";
      }
      else {
        if (is_scalar($op[0])) {
          $join .= ' ON ' . "$op[0] " . (!empty($op[2]) ? $op[2] : '=') . " $op[1]";
        }
        else {
          $o = '';
          $on = '';
          do {
            if (!empty($o)) {
              $on .= (!empty($o[3]) ? $o[3] : ' AND ');
            }
            $o = array_shift($op);
            $on .= "$o[0] " . (!empty($o[2]) ? $o[2] : '=') . " $o[1]";
          } while(count($op) > 0);
          $join .= " ON ($on)";
        }
      }
    }
    else {
      foreach($table as $t){
        if (!empty($join)) {
          $join .= ' ';
        }
        $join .= $this->join($t[0], $t[1]);;
      }
    }

    return $join;
  }
}

/* Todo:
 * Add functionality for different types of Join Clauses
 */
 