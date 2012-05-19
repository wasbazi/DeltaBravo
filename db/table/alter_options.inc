<?php
// TODO:
// 1) Ad other Alter Options
// 2) Read through code, hasn't been tested or read after coding
class GC_DB_AlterOptions extends GC_DB_CreateOptions{

	protected $actions = array(
		'ADD' => array(
      'COLUMN',
    ),
		'MODIFY' => array(
      'COLUMN',
    ),
    'CHANGE' => array(
      'COLUMN',
    ),
		'DROP' => array(
      'COLUMN',
    ),
	);

	public function col($options){
		for($i = 0; $i < count($options) ; $i++){
      $upper_option[0] = strtoupper($options[$i][0]);
      $upper_option[1] = strtoupper($options[$i][1]);
			if(isset($actions[$upper_option[0]])){
				$action = strtolower($options[$i][0]);
      }
      if(in_array($upper_option[1], $actions["$action"][0])){
        $type = $upper_options[1];
      }
      if($action && $type){
        $specifications = array_shift($options[$i]);
        $this->col .= (isset($this->col)) ? ' ,' : '';
        $this->col .= call_user_func_array(array($this,$action), $specifications);
      }else{
				continue;
			}
		}
	}
  
  // $name = (string)
  // $definition = array( (string) type, (string) length )
  // $position = 'First' | array( 'After', (string) col_nam )
  public function add($type, $name, $definition, $position = null) {
    switch($type) {
      case 'COLUMN':
        $add = "COLUMN '$name'";
        $add .= parent::col($definition);
        if($position && ( strtoupper($position) == 'FIRST' || strtoupper($position[0]) == 'AFTER')){
          $add .= (is_array($position)) ? $position[0] . ' ' . $position[1] : $position;
        }
    }
    return $add;
  }
  
  // $name = (string)
  public function drop($type, $name) {
    switch($type) {
      case 'COLUMN':
        $drop = 'COLUMN ' . $name;
    }
    return $drop;
  }
  
  // $name = array( (string) old col_name, (string) new col_name )
  public function change($type, $name, $definition, $position = null) {
    switch($type) {
      case 'COLUMN':
        $change = "COLUMN '" . $name[0] . ' ' . $name[1] . "'";
        $change .= parent::col($definition);
        if($position && ( strtoupper($position) == 'FIRST' || strtoupper($position[0]) == 'AFTER')){
          $change .= (is_array($position)) ? $position[0] . ' ' . $position[1] : $position;
        }
    }
    return $change;
  }
  
  // $name = (string) col_name
  public function modify($type, $name, $definition, $position = null) {
    switch($type) {
      case 'COLUMN':
        $modify = "COLUMN '$name'";
        $modify .= parent::col($definition);
        if($position && ( strtoupper($position) == 'FIRST' || strtoupper($position[0]) == 'AFTER')){
          $modify .= (is_array($position)) ? $position[0] . ' ' . $position[1] : $position;
        }
    }
    return $add;
  }

}