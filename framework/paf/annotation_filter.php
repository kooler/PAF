<?php
/**
 * Class for working with annotations
 * Singleton
 */
class AnnotationFilter {
	private static $instance = null;
	
	private function __construct() {}
	/**
	 * Get singleton instance
	 */
	public static function getInstance() {
		if (self::$instance == null) {
			self::$instance = new AnnotationFilter();
		}
		return self::$instance;
	}
	/**
	 * Get call type and method from annotation
	 * @return array((Before|After|Around),method name) or null
	 */
	public function getTypeMethodFromAnnotation($annotation) {
		$result = null;
		if (preg_match('/@(Before|After|Around)\((.*)\)/iU', $annotation, $matches)) {
			$result = array($matches[1], $matches[2]);
		}
		return $result;
	}
}
