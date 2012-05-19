<?php

class GC_DB_Modifier{
  public function group($column, $format = ''){
    return "GROUP BY $column $format";
  }
  
  public function order($column, $format = 'asc'){
    return "ORDER BY $column $format";
  }
  
  public function limit($limit_number, $offset = '0'){
    return "LIMIT $offset, $limit_number";
  }
}