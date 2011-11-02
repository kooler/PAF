<?php
/**
 * Intercept exception doesn't support multiply interceptions
 * for one method/function
 * This class should take this function and provide intercept chains
 * Singleton
 */
class InterceptionChain {
	private static $instance = null;
	/**
	 * List of functions that will be called
	 * Format: array('intercepted_object->method' => array('joinpoint_func1', 'joinpoint_func2'))
	 */
	private $interceptors = array();
	private $registeredInterceptFuncs = array();

public function getInterceptors() {return $this->interceptors;}

	private function __construct() {}
	/**
	 * Get singleton instance
	 */
	public static function getInstance() {
		if (self::$instance == null) {
			self::$instance = new InterceptionChain();
		}
		return self::$instance;
	}
	/**
	 * Add interceptor function
	 * @param $func 	 name of the function that should be intercepted
	 * @param $joinpoint Joinpoint object
	 */
	public function addInterception($func, Joinpoint $jp) {
		$this->interceptors[$func][] = $jp;
	}
	/**
	 * Call intercepting functions
	 * @param $type the type of the interceptor (Before|After|Around)
	 * @param $func name of the function that should be intercepted
	 */
	public function fire($type, $func, $arguments) {
		if (isset($this->interceptors[$func])) {
			foreach ($this->interceptors[$func] as $joinpoint) {
				if ($joinpoint->getCallType() == $type || $joinpoint->getCallType() == 'Around') {
					$joinpoint->fire($arguments);
				}
			}
		}
	}
	/**
	 * Get name of the common interceptors function
	 * @param $type the type of the interceptor (Before|After|Around)
	 * @param $func name of the function that should be intercepted
	 * @return name of the function
	 */
	private function getCommonInterceptorsFunctionName($type, $func) {
		return '__intercept_'.strtolower($type).'_'.preg_filter('/[^a-zA-Z]/i', '', $func);
	}
	/**
	 * Get common interception function
	 * @param $type the type of the interceptor (Before|After|Around)
	 * @param $func name of the function that should be intercepted
	 * @return name of the function that include whole chain of functions that must be called
	 */
	public function getMethodInterceptorsCall($type, $func) {
		$funcName = $this->getCommonInterceptorsFunctionName($type, $func);
		if (!function_exists($funcName)) {
			eval('function '.$funcName.'(){InterceptionChain::getInstance()->fire("'.$type.'","'.$func.'", func_get_args());}');
		}
		return $funcName;
	}
	/**
	 * Check where function has registered interceptors
	 * @param $type the type of the interceptor to search for (Before|After|Around)
	 * @param $func name of the function that should be intercepted
	 * @return true|false
	 */
	public function hasInterceptors($type, $func) {
		$funcName = $this->getCommonInterceptorsFunctionName($type, $func);
		return function_exists($func);
	}
	/**
	 * Register intercept function
	 * @param $name function name
	 */
	public function registerInterceptFunction($name) {
		$this->registeredInterceptFuncs[] = $name;
	}
	/**
	 * Check where function has been registered
	 * @param $name function name
	 * @return true|false
	 */
	public function IsRegistered($name) {
		return in_array($name, $this->registeredInterceptFuncs);
	}

}
