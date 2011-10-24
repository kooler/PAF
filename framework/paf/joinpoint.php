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
	 * Execute $call function
	 * This method will be fired on the $callType event for $objectToIntercept
	 */
	public function fire() {}
	/**
	 * Get name of the class which method should be intercepted
	 * @return class name or null if $objectToIntercept holds function name
	 */
	public function getClassName() {
		$className = null;
		$pointerPosition = strpos($this->objectToIntercept, '->');
		if ($pointerPosition > 0) {
			$className = substr($this->objectToIntercept, 0, $pointerPosition);
		}
		return $className;
	}
	/**
	 * Get name method that should be intercepted
	 * @return class name
	 */
	public function getMethodName() {
		$methodName = $this->objectToIntercept;
		$pointerPosition = strpos($this->objectToIntercept, '->') + 2;
		if ($pointerPosition > 2) {
			$methodName = substr($this->objectToIntercept, $pointerPosition, strlen($this->objectToIntercept) - $pointerPosition);
		}
		return $methodName;
	}
	/**
	 * Get call type
	 * @return call type
	 */
	public function getCallType() {
		return $this->callType;
	}
	/**
	 * Get function that must be called
	 */
	public function getCall() {
		return $this->call;
	}
}
