<?php

class DB_Condition {
  
  // Parses where statements nested in arrays
  public function parse_where($clause){
    // If this is an array then there is a clause
    if(isset($clause[0]) && is_array($clause[0]) && !is_array($clause[0][0])){ 
      $key = $clause[0][0];
      $value = $clause[0][1];
      $conditional = $clause[0][2];
      
      // Used in places such as IN (x, y, z)
      if(is_array($value)){
        $tmp = "";
        for( $i=0; $i < count($value); $i++){
          $tmp .= ($i > 0) ? ", " : "";
          $tmp .= $value[$i];
        }
        $value = $tmp;
      }else{
        $value = "'$value'";
      }

      $clause_frag = "( $key $conditional $value )";
      
      if(isset($clause[0][3])){ // Concatenate logical connector
        $clause_frag = " " . $clause[0][3] . " " . $clause_frag;
      }
      
      if(isset($clause[1])){ // Sibling array needs to be parsed
        array_shift($clause);
        return $this->parse_where($clause) . $clause_frag;
      }
      else{ // No sibling array exists 
        return $clause_frag;
      }
    }
    // If arrays are nested two deep then this a clause with a nested clause
    elseif(isset($clause[0]) && is_array($clause[0]) && is_array($clause[0][0])){
      // Used to concatenate logical connectors
      $length = count($clause[0]);
      $prefix = "";
      if(is_string($clause[0][--$length])){
        $prefix = ' ' . $clause[0][$length--] . ' ';
      }
      
      if(isset($clause[1])){
        return '(' . $this->parse_where($clause[1]) . ')' . $prefix . '(' . $this->parse_where($clause[0]) . ')';
      }else{
        return $prefix . '(' . $this->parse_where($clause[0]) . ')';
      }
    }
  }
  
  public function where($key = null, $value = null, $conditional = '='){
    
    // Ensure where clause is in array format for foreach clause
    if(is_array($key)){ // If $key is an array then multiple where clauses
      $clause_array = $key;    
      $this->where = $this->parse_where($clause_array);
    }else{ // Else, single where clause and it needs to be stored in an array
      $condition = "$key $conditional ?";
      $placeholders = array($value);
      $this->where = $condition;
      $this->placeholders = $placeholders;
    }
    $this->where = "WHERE " . $this->where;
    //echo $this->where;
    // Returns where clause as string, implode multiple where clauses with " AND "
    return $this;
  }
}
