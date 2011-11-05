<?php
/**
 * Class for aspects management
 * Singleton
 */
class AspectRegistry {
	private static $instance = null;
	private $aspects;
	private $joinpoints;
	/**
	 * Get list of joinpoints and save them to local variable
	 */
	private function __construct() {
		//Load Joinpoints - each joinpoint should implements Joinpoint interface
		$this->joinpoints = array();
	}
	/**
	 * Get singleton instance
	 */
	public static function getInstance() {
		if (self::$instance == null) {
			self::$instance = new AspectRegistry();
		}
		return self::$instance;
	}
	/**
	 * Add aspect
	 * @param Aspect aspect object
	 */
	public function addAspect(Aspect $aspect) {
		$this->processAspect($aspect);
		$this->aspects[] = $aspect;
	}
	/**
	 * Add all aspect joinpoints to local array
	 * @param Aspect aspect class
	 */
	private function processAspect(Aspect $aspect) {
		$className = get_class($aspect);
		$asp = new ReflectionClass($className);
		foreach ($asp->getMethods() as $joinpoint) {
			$joinInfo = AnnotationFilter::getInstance()->getTypeMethodFromAnnotation($joinpoint->getDocComment());
			if ($joinInfo != null) {
				$this->joinpoints[] = new Joinpoint($joinInfo[0], $joinInfo[1], $className.'->'.$joinpoint->name);
			}
		}
	}
	/**
	 * Add intercepters for all joinpoints
	 */
	public function interceptAll() {
		if (count($this->joinpoints) > 0) {
			foreach ($this->joinpoints as $joinpoint) {
				/* If joinpoint contails method interception get 
				 * all classes that matches joinpoint class filter */
				$className = $joinpoint->getClassName();
				if ($className != null) {
					$classes = ClassReflection::getInstance()->getClassesByMask($className);
					/* Add interception for all methods in found classes 
					 * that matches joinpoint method filter */
					if (count($classes) > 0) {
						$methodName = $joinpoint->getMethodName();
						foreach ($classes as $class) {
							$methods = ClassReflection::getInstance()->getClassMethodsByMask($class, $methodName);
							//Add interception for each class->method pair
							if (count($methods) > 0) {
								foreach ($methods as $method) {
									//Add intercept to chain
									InterceptionChain::getInstance()->addInterception($class.'->'.$method->name, $joinpoint);
									if ($joinpoint->getCallType() == 'Before' || $joinpoint->getCallType() == 'Around') {
										$func = InterceptionChain::getInstance()->getMethodInterceptorsCall('Before', $class.'->'.$method->name);
										if (!InterceptionChain::getInstance()->IsRegistered($func)) {
											intercept_add($class.'->'.$method->name, $func, PRE_INTERCEPT);
											InterceptionChain::getInstance()->registerInterceptFunction($func);
										}
									}
									if ($joinpoint->getCallType() == 'After' || $joinpoint->getCallType() == 'Around') {
										$func = InterceptionChain::getInstance()->getMethodInterceptorsCall('After', $class.'->'.$method->name);
										if (!InterceptionChain::getInstance()->IsRegistered($func)) {
											intercept_add($class.'->'.$method->name, $func, POST_INTERCEPT);
											InterceptionChain::getInstance()->registerInterceptFunction($func);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	
}
