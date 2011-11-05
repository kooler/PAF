<?php
/**
 * Wrapper for reflection API
 * Singleton
 */
class ClassReflection {
	private static $instance = null;
	private $classes = array();
	/**
	 * Get list of all classes and save it to local variable
	 */
	private function __construct() {
		//Get classes list
		$this->classes = get_declared_classes();
	}
	/**
	 * Get singleton instance
	 */
	public static function getInstance() {
		if (self::$instance == null) {
			self::$instance = new ClassReflection();
		}
		return self::$instance;
	}
	/**
	 * Get all classes filtered by regexp mask
	 * @param $mask regexp mask
	 * @return array with filtered class names
	 */
	public function getClassesByMask($mask) {
		$filtered = array();
		foreach ($this->classes as $className) {
			if (preg_match('/'.$mask.'/i', $className)) {
				$filtered[] = $className;
			}
		}
		return $filtered;
	}
	/**
	 * Get methods of some class that matched mask
	 * @param $class	class name
	 * @param $method	method name mask
	 * @return array with filtered methods as ReflectionMethod instances
	 */
	public function getClassMethodsByMask($class, $mask) {
		//Create reflection class
		$class = new ReflectionClass($class);
		$filtered = array();
		//Check each class method
		foreach ($class->getMethods() as $method) {
			if (preg_match('/'.$mask.'/i', $method->name)) {
				//Add comment to method object for annotation parsing
				$method->comment = $method->getDocComment();
				$filtered[] = $method;
			}
		}
		return $filtered;
	}
	/**
	 * Avoid cloning of singleton object
	 */
	public function __clone() {
		trigger_error('Singleton cannot be cloned');
	}
}
