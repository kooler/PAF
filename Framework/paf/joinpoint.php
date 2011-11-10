<?php
/**
 * Joinpoint interface
 * Each joinpoint should extend it
 */
class Joinpoint {
	/**
	 * Type of the call: Before, After, Arround
	 * @string
	 */
	private $callType;
	/**
	 * Object/method to intecept(for ex. myclass->mymethod)
	 * @string
	 */
	private $objectToIntercept;
	/**
	 * Method that must be called
	 * @string
	 */
	private $call;
	/**
	 * @param $callType				Type of the call
	 * @param $objectToIntercept	Object/method to intecept
	 * @param $call					Method that must be called
	 */
	function __construct($callType, $objectToIntercept, $call) {
		$this->callType = $callType;
		$this->objectToIntercept = $objectToIntercept;
		$this->call = $call;
	}
	/**
	 * Get name of the class from interpret string
	 * @param $string class->method string
	 * @return class name or null if string holds function name
	 */
	private function _getClassName($string) {
		$className = null;
		$pointerPosition = strpos($string, '->');
		if ($pointerPosition > 0) {
			$className = substr($string, 0, $pointerPosition);
		}
		return $className;
	}
	/**
	 * Get name of the method from interpret string
	 * @param $string class->method string
	 * @return method or function name
	 */
	private function _getMethodName($string) {
		$methodName = $this->objectToIntercept;
		$pointerPosition = strpos($string, '->') + 2;
		if ($pointerPosition > 2) {
			$methodName = substr($string, $pointerPosition, strlen($string) - $pointerPosition);
		}
		return $methodName;
	}
	/**
	 * Get name of the class which method should be intercepted
	 * @return class name or null if $objectToIntercept holds function name
	 */
	public function getClassName() {
		return $this->_getClassName($this->objectToIntercept);
	}
	/**
	 * Get name method that should be intercepted
	 * @return class name
	 */
	public function getMethodName() {
		return $this->_getMethodName($this->objectToIntercept);
	}
	/**
	 * Get name of the class which method should be called
	 * @return class name or null if $objectToIntercept holds function name
	 */
	public function getCallClassName() {
		return $this->_getClassName($this->call);
	}
	/**
	 * Get name method that should be called
	 * @return class name
	 */
	public function getCallMethodName() {
		return $this->_getMethodName($this->call);
	}
	/**
	 * Get call type
	 * @return call type
	 */
	public function getCallType() {
		return $this->callType;
	}
	/**
	 * Execute $call function
	 * This method will be fired on the $callType event for $objectToIntercept
	 */
	public function fire($arguments) {
		$objClass = $this->getCallClassName();
		$obj = new $objClass;
		$obj->{$this->getCallMethodName()}($arguments);
	}
	/**
	 * Get function that must be called
	 */
	public function getCall() {
		return $this->call;
	}
}
