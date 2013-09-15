<?php
/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

$GT_LIB_VERSION = "1.1.0";
$GT_LIB_DATETIME = "201308101512";

// Constants ++
$GT_STATUS_OK = "OK";
$GT_STATUS_ERROR = "ERROR";
$GT_STATUS_ERROR_PLEASE_LOGIN = "PLEASE LOGIN!";
$GT_STATUS_ERROR_YOU_ARE_NOT_ADMIN = "YOU ARE NOT ADMIN!";
$GT_STATUS_ERROR_NO_SOURCE = "NO SOURCE!";
$GT_STATUS_ERROR_NOT_REGISTERED_EMAIL = "NOT REGISTERED EMAIL!";
$GT_STATUS_ERROR_WRONG_PASSWORD = "WRONG PASSWORD!";
$GT_STATUS_ERROR_INSERT = "INSERT ERROR!";
$GT_STATUS_ERROR_QUERY = "QUERY ERROR!";
$GT_STATUS_ERROR_UPDATE = "UPDATE ERROR!";
$GT_STATUS_ERROR_DELETE = "DELETE ERROR!";
$GT_STATUS_ERROR_NO_ACCESS_RIGHT = "NO ACCESS RIGHT!";
$GT_STATUS_ERROR_ALREADY_EXIST = "ALREADY EXIST!";

$GT_SUCCESS = 1;
$GT_ERROR = 0;

$GT_TRUE = 1;
$GT_FALSE = 0;

$GT_ACCOUNT_TYPE_ERROR = 0;
$GT_ACCOUNT_TYPE_GUEST = 1;
$GT_ACCOUNT_TYPE_USER  = 2;
$GT_ACCOUNT_TYPE_ADMIN = 3;

$GT_ACCOUNT_STATUS_OK = 1;
$GT_ACCOUNT_STATUS_ERROR = 0;
$GT_ACCOUNT_STATUS_EMAIL_NOT_VERIFIED = -2;

$GT_CREDIT_EVENT_TOP_UP = 1;
$GT_CREDIT_EVENT_BUY_EBOOK = 2;

$GT_DIR_TYPE_DIR = 1;
$GT_DIR_TYPE_FILE = 2;
$GT_DIR_TYPE_TEXT = 3;
$GT_DIR_TYPE_PHOTO = 4;
$GT_DIR_TYPE_VIDEO = 5;
$GT_DIR_TYPE_AUDIO = 6;
$GT_DIR_TYPE_PDF = 7;
$GT_DIR_TYPE_EPUB = 8;
$GT_DIR_TYPE_ALBUM = 9;
$GT_DIR_TYPE_AUDIOBOOK = 10;
$GT_DIR_TYPE_CHAT = 11;
$GT_DIR_TYPE_ARTICLE = 12;
$GT_DIR_TYPE_DIR_CONTENT = 13;
$GT_DIR_TYPE_PROFILE = 14;
$GT_DIR_TYPE_YOUTUBE = 15;
$GT_DIR_TYPE_MEMBER = 16;
$GT_DIR_TYPE_STORE = 17;
$GT_DIR_TYPE_EBOOK = 18;

$GT_DIR_STATUS_PUBLISH = 1;
$GT_DIR_STATUS_DRAFT = 0;
$GT_DIR_STATUS_PUBLISH_HIDE = 2;
$GT_DIR_STATUS_UNPUBLISH = -1;

$GT_FILE_TYPE_URL = 1;
$GT_FILE_TYPE_FILE = 2;
$GT_FILE_TYPE_TEXT = 3;
$GT_FILE_TYPE_PHOTO = 4;
$GT_FILE_TYPE_VIDEO = 5;
$GT_FILE_TYPE_AUDIO = 6;
$GT_FILE_TYPE_PDF = 7;
$GT_FILE_TYPE_EPUB = 8;
$GT_FILE_TYPE_ZIP = 9;
$GT_FILE_TYPE_THUMBNAIL = 10;
$GT_FILE_TYPE_TEMP = 11;

$GT_LANGUAGE_UNKNOWN = 0;
$GT_LANGUAGE_EN = 1;
$GT_LANGUAGE_ZH_TW = 2;
$GT_LANGUAGE_ZH_CN = 3;

$GT_LOG_LEVEL_NONE = 0;
$GT_LOG_LEVEL_VERBOSE = 1;
$GT_LOG_LEVEL_DEBUG = 2;
$GT_LOG_LEVEL_INFO = 3;
$GT_LOG_LEVEL_WARN = 4;
$GT_LOG_LEVEL_ERROR = 5;
$GT_LOG_LEVEL_ASSERT = 6;

$GT_LOG_TYPE_NONE = 0;
$GT_LOG_TYPE_VIEW = 1;
$GT_LOG_TYPE_DOWNLOAD = 2;
$GT_LOG_TYPE_LOGIN = 3;
$GT_LOG_TYPE_REGISTER = 4;
$GT_LOG_TYPE_ADMIN = 5;
$GT_LOG_TYPE_EDIT = 6;
$GT_LOG_TYPE_LOGOUT = 7;
$GT_LOG_TYPE_DELETE = 8;
$GT_LOG_TYPE_CREATE = 9;
$GT_LOG_TYPE_ATTACK = 10;
$GT_LOG_TYPE_MAIL = 11;
$GT_LOG_TYPE_PASSWORD = 12;
$GT_LOG_TYPE_ACCOUNT = 13;
$GT_LOG_TYPE_SMTP = 14;
$GT_LOG_TYPE_SUBSCRIBE = 15;
$GT_LOG_TYPE_UPLOAD = 16;
$GT_LOG_TYPE_BLACK_LIST = 17;
$GT_LOG_TYPE_SEARCH = 18;
$GT_LOG_TYPE_WHITE_LIST = 19;
$GT_LOG_TYPE_MEMBER = 20;
// Constants --

// Local Parameters ++
$creditExpireTime = 3600 * 24 * 365;

// Local Parameters --

$pageTimerStart = GtTime_GetGmtTimeSecMicro();

$dbLink = mysql_pconnect($dbAddress, $dbUser, $dbPassword);
if ($dbLink == FALSE) {
	$dbLink = mysql_pconnect($dbAddress, $dbUser, $dbPassword);
}
if ($dbLink == FALSE) {
	die("dbLink == FALSE");
}

mysql_query("SET CHARACTER SET 'UTF-8'");
mysql_query("SET NAMES 'UTF8'");

if (mysql_select_db ($dbSchema, $dbLink) == FALSE) {
	$dbLink = mysql_pconnect($dbAddress, $dbUser, $dbPassword);
	if (mysql_select_db ($dbSchema, $dbLink) == FALSE) {
		die("mysql_select_db (dbSchema, dbLink) == FALSE");
	}
}
$pageTimerDbInit = GtTime_GetGmtTimeSecMicro();

if ($serverBlackListCheck == 1) {
	$ip = $_SERVER["REMOTE_ADDR"];
	if (GtBlackList_Check($ip)) {
		GtLog_W($GT_LOG_TYPE_BLACK_LIST);
		die("WARNING! Your IP ($ip) is in black list! Don't attack this website!");
	}
}

include(getenv("DOCUMENT_ROOT")."/includePAX4QF98/safeMode.php");
if($serverSafeModeOff != 1) {
	$safeMode = new safeMode();
	$safeMode->xss(2, 'log_attack.html');
}

$pageLang = GtInput_Secure($_COOKIE["cookieLang"]);
if($pageLang == NULL) {
	$pageLang = strtolower($_SERVER["HTTP_ACCEPT_LANGUAGE"]);
	if(ereg("zh-tw", $pageLang)) {
		$pageLang = "zh_TW";
	} else if(ereg("zh-cn", $pageLang)) {
		$pageLang = "zh_CN";
	} else {
		$pageLang = "en_US";
	}
}

include(getenv("DOCUMENT_ROOT")."/includePAX4QF98/chinese.php");
$strlen = mb_strlen($serverTC);
$strpos = 0;
while ($strlen > 0) {
	$tc = mb_substr($serverTC, $strpos, 1, "UTF-8");
	$sc = mb_substr($serverSC, $strpos, 1, "UTF-8");
	
	$serverToTC[$sc] = $tc;
	$serverToSC[$tc] = $sc;
	
	$strpos ++;
	$strlen --;
}

// Functions ++

function GtAccount_CheckEmailAddress($email) {
	//$atIndex = strrpos($email, "@");
	//if (is_bool($atIndex) && !$atIndex) {
	//	return FALSE;
	//} else {
		$domain = substr($email, $atIndex + 1);
		$local = substr($email, 0, $atIndex);
		$localLen = strlen($local);
		$domainLen = strlen($domain);
		/*if ($localLen < 1 || $localLen > 64) {
			// local part length exceeded
			return -2;
		} else if ($domainLen < 1 || $domainLen > 255) {
			// domain part length exceeded
			return -3;
		} else if ($local[0] == '.' || $local[$localLen-1] == '.') {
			// local part starts or ends with '.'
			return -4;
		} else if (preg_match('/\\.\\./', $local)) {
			// local part has two consecutive dots
			return -5;
		} else if (!preg_match('/^[A-Za-z0-9\\-\\.]+$/', $domain)) {
			// character not valid in domain part
			return -6;
		} else if (preg_match('/\\.\\./', $domain)) {
			// domain part has two consecutive dots
			return -7;
		} else if(!preg_match('/^(\\\\.|[A-Za-z0-9!#%&`_=\\/$\'*+?^{}|~.-])+$/', str_replace("\\\\","",$local))) {
			// character not valid in local part unless 
			// local part is quoted
			if (!preg_match('/^"(\\\\"|[^"])+"$/', str_replace("\\\\","",$local))) {
				return -8;
			}
		}
	
		if (!(checkdnsrr($domain,"MX") || checkdnsrr($domain,"A"))) {
			// domain not found in DNS
			return -9;
		}*/
		$regexp = "^([_a-z0-9-]+)(\.[_a-z0-9-]+)*@([a-zA-Z0-9-]+)(\.[a-zA-Z0-9-]+)*(\.[a-zA-Z0-9]{2,4})$"; 
	    if (eregi($regexp, $email)) { 
	        //list($user, $domain) = split("@", $email);  
	        //判斷該網址是否存在，建議使用checkdnsrr或getmxrr，但win32下無法使用 
	        //if (gethostbyname($domain) != $domain)  
	            return 1; 
	    } 
	//}	
	return 0;
	/*$regexp = "^([_a-z0-9-]+)(\.[_a-z0-9-]+)*@([a-z0-9-]+)(\.[a-z0-9-]+)*(\.[a-z]{2,4})$"; 
	    if (eregi($regexp, $email)) { 
	        list($user, $domain) = split("@", $email);  
	        //判斷該網址是否存在，建議使用checkdnsrr或getmxrr，但win32下無法使用 
	        if (gethostbyname($domain) != $domain)  
	            return true; 
	    } 
	 
	    return FALSE; */
}

function GtAccount_CheckMobilePhoneNumber($companyPhone) {
	$atIndex = strrpos($phoneMobile, "+");
	if (is_bool($atIndex) && !$atIndex) {
		return 0;
	}   
	return 1;
}

function GtAccount_GetNameDisplay($accountSn) {
	global $dbLink;
	
	$sql = "SELECT nameDisplay FROM account WHERE sn ='$accountSn'";
	$result = mysql_query($sql, $dbLink);
	$row = mysql_fetch_array($result);
	if($row != FALSE) {
		return $row[nameDisplay];
	}
	
	return "";
}

function GtAccount_GetPasswordSha256($password) {
	global $serverAccountPasswordSha256Prefix;
	
	return hash("sha256", $serverAccountPasswordSha256Prefix.$password); 
}

function GtAccount_GetSn() {
	global $serverTimeoutCookie;
	global $serverUrl;
	
	$cookieSessionSn = (int)GtInput_Secure($_COOKIE["cookieSessionSn"]);
	$cookieSessionToken = GtInput_Secure($_COOKIE["cookieSessionToken"]);
	$accountSn = GtAccount_GetSnFromSession($cookieSessionSn, $cookieSessionToken);
	
	if ($serverHttps == "https") {
		setcookie("cookieSessionSn", $cookieSessionSn, time() + $serverTimeoutCookie, '/', $serverUrl, TRUE, TRUE);
		setcookie("cookieSessionToken", $cookieSessionToken, time() + $serverTimeoutCookie, '/', $serverUrl, TRUE, TRUE);
	} else {
		setcookie("cookieSessionSn", $cookieSessionSn, time() + $serverTimeoutCookie, '/', $serverUrl);
		setcookie("cookieSessionToken", $cookieSessionToken, time() + $serverTimeoutCookie, '/', $serverUrl);
	}
	
	return $accountSn;
}

function GtAccount_GetSnFromId($accountId) {
	global $dbLink;
	
	$sql = "SELECT sn FROM account WHERE email ='$accountId'";
	$result = mysql_query($sql, $dbLink);
	$row = mysql_fetch_array($result);
	if($row != FALSE) {
		return $row[sn];
	}
	
	return 0;
}

function GtAccount_GetSnFromSession($sessionSn, $sessionToken) {
	global $dbLink;
	global $serverTimeoutLogin;
	global $serverTimeZone;
	
	if($sessionSn == NULL) {
		return -1;
	}
	
	if($sessionToken == NULL) {
		return -2;
	}
	
	$timeSecNow = GtTime_GetGmtTimeSec();
	$timeSecValid = $timeSecNow - $serverTimeoutLogin;
	
	$sql = "SELECT token, timeSec, accountSn FROM accountSession WHERE sn='$sessionSn' AND timeSec > '$timeSecValid'";
	$result = mysql_query($sql, $dbLink);
	$row = mysql_fetch_array($result);
	if($row == FALSE) {
		return -3;
	}
	$token = $row[token];
	$timeSec = $row[timeSec];
	$accountSn = $row[accountSn];
    
	if($token != $sessionToken) {
		return -8;
	}

	$sql = "UPDATE accountSession SET timeSec='$timeSecNow' WHERE sn='$sessionSn'";
	$result = mysql_query($sql, $dbLink);
	if($result == FALSE) {
		return -10;
	}
	return $accountSn;
}

function GtAccount_GetSnFromSessionWithoutToken($sessionSn) {
	global $dbLink;
	global $serverTimeoutLogin;
	global $serverTimeZone;
	
	if($sessionSn == NULL) {
		return -1;
	}
		
	$timeSecNow = GtTime_GetGmtTimeSec($serverTimeZone);
	$timeSecValid = $timeSecNow - $timeoutLogin;
	
	$sql = "SELECT accountSn FROM accountSession WHERE sn='$sessionSn' AND timeSec > '$timeSecValid'";
	$result = mysql_query($sql, $dbLink);
	$row = mysql_fetch_array($result);
	if($row == FALSE) {
		return -3;
	}
	$accountSn = $row[accountSn];
	return $accountSn;
}

function GtAccount_GetStatus($accountId) {
	global $dbLink;
	global $GT_ACCOUNT_STATUS_ERROR;
	
	$sql = "SELECT status FROM account WHERE email ='$accountId'";
	$result = mysql_query($sql, $dbLink);
	$row = mysql_fetch_array($result);
	if($row != FALSE) {
		return $row[status];
	}
	
	return $GT_ACCOUNT_STATUS_ERROR;
}

function GtAccount_GetType($accountSn) {
	global $dbLink;
	
	$sql = "SELECT type FROM account WHERE sn ='$accountSn'";
	$result = mysql_query($sql, $dbLink);
	$row = mysql_fetch_array($result);
	if($row != FALSE) {
		return $row[type];
	}
	
	return 0;
}

function GtBlackList_Add($ip) {
	global $dbLink;
	
	$timeSec = GtTime_GetGmtTimeSec() - 86400;
	$sql = "DELETE FROM blackList WHERE timeSec < '$timeSec'";
	$result = mysql_query($sql, $dbLink);
	if($result == FALSE) {
		die("Delete blackList error!");
	}
	
	$sql = "SELECT sn FROM blackList WHERE ip = '$ip'";
	$result = mysql_query($sql, $dbLink);
	$row = mysql_fetch_array($result);
	if($row != FALSE) {
		return 2;
	}
	
	$timeSec = GtTime_GetGmtTimeSec();
	$sql = "INSERT INTO blackList (ip, timeSec) values('$ip', '$timeSec')";
	$result = mysql_query($sql, $dbLink);
	if($result == FALSE) {
		return 0;	
	}

	return 1;	
}

function GtBlackList_Check($ip) {
	global $dbLink;
	
	$sql = "SELECT sn FROM blackList WHERE ip = '$ip'";
	$result = mysql_query($sql, $dbLink);
	$row = mysql_fetch_array($result);
	if($row == FALSE) {
		return 0;
	}
	
	$sql = "SELECT sn FROM whiteList WHERE ip = '$ip'";
	$result = mysql_query($sql, $dbLink);
	$row = mysql_fetch_array($result);
	if($row == FALSE) {
		return 1;
	}
	
	return 0;
}

function GtChinese_ToSC($strTC) {
	$strlen = mb_strlen($strTC);
	if($strlen == 0) {
		return "";
	}
	
	global $serverToSC;
	
	$strpos = 0;
	while ($strlen > 0) {
		$tc = mb_substr($strTC, $strpos, 1, "UTF-8");
		$sc = $serverToSC[$tc];
		
		if(mb_strlen($sc) > 0) {
			$strSC = $strSC . $sc;
		} else {
			$strSC = $strSC . $tc;
		}
		
		$strpos ++;
		$strlen --;
	}
	if ($strSC == "关斓光华") {
		$strSC = "关于光华";	
	}
	
	return $strSC;
}

function GtChinese_ToTC($strSC) {
	$strlen = mb_strlen($strSC);
	if($strlen == 0) {
		return "";
	}
	
	global $serverToTC;
	
	$strpos = 0;
	while ($strlen > 0) {
		$sc = mb_substr($strSC, $strpos, 1, "UTF-8");
		$tc = $serverToTC[$sc];
		
		if(mb_strlen($tc) > 0) {
			$strTC = $strTC . $tc;
		} else {
			$strTC = $strTC . $sc;
		}
		
		$strpos ++;
		$strlen --;
	}
	
	return $strTC;
}

function GtClient_GetIP() {  
	if (isset($HTTP_SERVER_VARS["HTTP_X_FORWARDED_FOR"])) {  
		$ip = $HTTP_SERVER_VARS["HTTP_X_FORWARDED_FOR"];
	} elseif (isset($HTTP_SERVER_VARS["HTTP_CLIENT_IP"])) {  
		$ip = $HTTP_SERVER_VARS["HTTP_CLIENT_IP"];  
    } elseif (isset($HTTP_SERVER_VARS["REMOTE_ADDR"])) {  
		$ip = $HTTP_SERVER_VARS["REMOTE_ADDR"];  
	} elseif (getenv("HTTP_X_FORWARDED_FOR")) {  
        $ip = getenv("HTTP_X_FORWARDED_FOR");  
    } elseif (getenv("HTTP_CLIENT_IP")) {  
        $ip = getenv("HTTP_CLIENT_IP");  
    } elseif (getenv("REMOTE_ADDR")) {  
        $ip = getenv("REMOTE_ADDR");  
    } else {  
        $ip = "";  
    }
    return $ip;  
}

function GtCsrf_CheckToken($sn, $token, $accountSn = 0) {
	global $dbLink;
	global $serverTimeoutCsrf;
	
	$sql = "SELECT token FROM csrf WHERE sn = '$sn' AND accountSn = '$accountSn'";
	$result = mysql_query($sql, $dbLink);
	$row = mysql_fetch_array($result);
	if($row == FALSE) {
		return -1;
	}
	
	if ($row[token] != $token) {
		return -2;	
	}
	
	/*
	$sql = "DELETE FROM csrf WHERE sn = '$sn'";
	$result = mysql_query($sql, $dbLink);
	if($result == FALSE) {
		die("Delete CSRF error!");
	}
	*/

	$timeSec = GtTime_GetGmtTimeSec() - $serverTimeoutCsrf;
	$sql = "DELETE FROM csrf WHERE timeSec < $timeSec";
	$result = mysql_query($sql, $dbLink);
	if($result == FALSE) {
		die("Delete old CSRF error!");
	}
	
	return 1;	
}

function GtCsrf_GetToken(&$sn, &$token, $accountSn = 0) {
	global $dbLink;
	
	$token = hash("sha256", rand().rand().rand().rand());
	$timeSec = GtTime_GetGmtTimeSec();
	$sql = "INSERT INTO csrf (token, timeSec, accountSn) values('$token', '$timeSec', '$accountSn')";
	$result = mysql_query($sql, $dbLink);
	if($result == FALSE) {
		return 0;	
	}
	$sn = mysql_insert_id();

	return 1;	
}

function GtCredit_GetBalance($accountSn) {
	global $dbLink;
	
	$sql = "SELECT balance FROM credit WHERE accountSn ='$accountSn'";
	$result = mysql_query($sql, $dbLink);
	$row = mysql_fetch_array($result);
	if($row != false) {
		$balance = $row[balance];
		
		return $balance;
	}
	
	return 0;
}

function GtCredit_Change($accountSn, $amount, $event = 0, $message = "", $dirSn = "", $dirName = "") {
	global $dbLink;
	global $creditExpireTime;
	
	$timeSec = GtTime_GetGmtTimeSec();
	$expireTimeSec = $timeSec + $creditExpireTime;
	
	$sql = "SELECT sn FROM credit WHERE accountSn ='$accountSn'";
	$result = mysql_query($sql, $dbLink);
	$row = mysql_fetch_array($result);
	if($row != false) {
		$sn = $row[sn];
		
		$sql = "UPDATE credit SET balance = balance + $amount, expireTimeSec = '$expireTimeSec' WHERE sn='$sn'";
		$result = mysql_query($sql, $dbLink);
		if($result == FALSE) {
  			return -1;
		}
	} else {
		$sql = "INSERT INTO credit (accountSn, currency, balance, timeSec, expireTimeSec) values('$accountSn', 'TWD', '$amount', '$timeSec', '$expireTimeSec')";
		$result = mysql_query($sql, $dbLink);
			if($result == FALSE) {
  				return -2;
		}
	}
	
	$sql = "SELECT balance FROM credit WHERE accountSn ='$accountSn'";
	$result = mysql_query($sql, $dbLink);
	$row = mysql_fetch_array($result);
	if($row == false) {
		return -3;
	}
	$balance = $row[balance];
	
	$sql = "INSERT INTO creditLog (accountSn, balance, timeSec, event, message, amount, dirSn, dirName) values('$accountSn', '$balance', '$timeSec', '$event', '$message', '$amount', '$dirSn', '$dirName')";
	$result = mysql_query($sql, $dbLink);
		if($result == FALSE) {
  			return -4;
	}
	
	return 1;
}

function GtDebug_I($dbgFrom, $message) {
	global $dbLink;
	
	$timeSec = GtTime_GetGmtTimeSec();
	$timeDateTime = GtTime_GetDateTime2($timeSec);
	$sql = "INSERT INTO debug (debugFrom, message, timeSec, timeDateTime) values('$dbgFrom', '$message', '$timeSec', '$timeDateTime')";
	$result = mysql_query($sql, $dbLink);
  	if($result == FALSE) {
		return 0;
	}
		
	return 1;
}

function GtDict_Trans($str, $from, $to) {
	if(strlen($str) == 0) {
		return "";
	}
	
	if(strlen($from) == 0) {
		return "";
	}
	
	if(strlen($to) == 0) {
		return "";
	}
	
	return $str;
}

function GtData_Bin2Hex($data) {
	return bin2hex($data);
}

function GtData_Hex2Bin($data) {
     $len = strlen($data);
     return pack("H" . $len, $data);
}

function GtEnc_Dec($encType, &$content, $contentSize, $key, $keySize, $keyShift) {
	if($encType == 0) {
		return 1;	
	}
	
    $ctr = $keyShift;
    for ($i = 0; $i < $contentSize; $i ++) {
        if ($ctr == $keySize) $ctr = 0;
		$content[$i] = ~($content[$i]^$key[$ctr]);
        $ctr++;
    }
    return 1;
}

function GtEnc_DecThenEnc($encType, &$content, $contentSize, $key, $keySize, $keyShift, $keyNew, $keyNewSize, $keyNewShift) {
	if($encType == 0) {
		return 1;	
	}
	
	$temp = 0x0;
    $ctr = $keyShift;
	$ctrNew = $keyNewShift;
    for ($i = 0; $i < $contentSize; $i ++) {
        if ($ctr == $keySize) $ctr = 0;
		if ($ctrNew == $keyNewSize) $ctrNew = 0;
		
		$temp = $content[$i]^$key[$ctr];
		$content[$i] = $temp^$keyNew[$ctrNew];
		
        $ctr++;
		$ctrNew++;
    }
	
    return 1;
}

function GtEnc_DownloadFile($sourceFileName, $fileName, $mimeType = "", $cacheEnable = 0, $cacheTimeSec = 0, $isAttachment = 1, $timeout = 0, $encType = 0, $key = "", $keySize = 0) {
	//Turn off output buffering to decrease cpu usage.
	ob_end_clean();
	
	// Set execution time limit
	set_time_limit($timeout);
			
	if(!is_readable($sourceFileName)) {
		return -1;
	}

	$fileSize = filesize($sourceFileName);
	
	$fileExtension = strtolower(substr(strrchr($fileName,"."), 1));
	$fileName = rawurldecode($fileName);

	/* Figure out the MIME type (if not specified) */
	$knownMimeTypes = array(
	"pdf" => "application/pdf",
	"txt" => "text/plain",
	"html" => "text/html",
	"htm" => "text/html",
	"exe" => "application/octet-stream",
	"zip" => "application/zip",
	"doc" => "application/msword",
	"xls" => "application/vnd.ms-excel",
	"ppt" => "application/vnd.ms-powerpoint",
	"gif" => "image/gif",
	"png" => "image/png",
	"jpeg"=> "image/jpg",
	"jpg" => "image/jpg",
	"php" => "text/plain",
	"mp4" => "video/mp4",
	"mp3" => "audio/mpeg"  // For IE
	//"mp3" => "audio/mpeg, audio/x-mpeg, audio/x-mpeg-3, audio/mpeg3"
	);

	if($mimeType == "") {
		if(array_key_exists($fileExtension, $knownMimeTypes)) {
			$mimeType = $knownMimeTypes[$fileExtension];
		} else {
			$mimeType = "application/force-download";
		};
	};
	
	if ($cacheEnable == 1) {
		header("Cache-Control: private, max-age=$cacheTimeSec");
		//header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");
	} else {
		header('Cache-Control: public, must-revalidate, max-age=0');
    	header('Pragma: no-cache'); 
		header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");
	}

	// Required for IE, otherwise Content-Disposition may be ignored.
	if(ini_get('zlib.output_compression')) {
		ini_set('zlib.output_compression', 'Off');
	}

	header("Content-Transfer-Encoding: binary");
	header("Accept-Ranges: bytes");
	header("Content-Type: " . $mimeType);
	header("Content-Description: File Transfer");
	if($isAttachment == 1) {
		header('Content-Disposition: attachment; filename="'.$fileName.'"');
	} else {
		header('Content-Disposition: inline; filename="'.$fileName.'"');
	}
	
	
	// Multipart-download and download resuming support.
	$rangeStart = 0;
	if(isset($_SERVER['HTTP_RANGE'])) {
		//fwrite($fp, $_SERVER['HTTP_RANGE']."\n");
		
		if (preg_match('/bytes=\h*(\d+)-(\d*)[\D.*]?/i', $_SERVER['HTTP_RANGE'], $matches))
		{
			$rangeStart = intval($matches[1]);
			if (!empty($matches[2]))
			{
				$rangeEnd = intval($matches[2]);
			}
		}
		
		if(strlen($rangeStart) == 0) {
			$rangeStart = 0;	
		}
		
		if($rangeStart < 0) {
			$rangeStart = 0;	
		}
		
		if(strlen($rangeEnd) == 0) {
			$rangeEnd = $fileSize - 1;	
		}
		
		if($rangeEnd < 0) {
			$rangeEnd = 0;	
		}
		
		if($rangeEnd >= $fileSize) {
			$rangeEnd = $fileSize - 1;
		}
		
		if($rangeEnd < $rangeStart) {
			$rangeEnd = $rangeStart;
		}

		$contentLength = $rangeEnd - $rangeStart + 1;
		
		header("HTTP/1.1 206 Partial Content");
		header("Content-Length: $contentLength");
		header("Content-Range: bytes ".$rangeStart."-".$rangeEnd."/".$fileSize);
	} else {
		$contentLength = $fileSize;
		header("Content-Length: $contentLength");
	}

	// Output the file itself.
	$chunkSize = 1048576; // 1MB buffer.
	$readSize = 0;
	$file = fopen($sourceFileName, 'rb');
	if ($file != FALSE) {
		if($rangeStart > 0) {
			fseek($file, $rangeStart);
		}

		while(!feof($file) && (!connection_aborted()) && ($readSize < $contentLength)) {
			$buffer = fread($file, $chunkSize);
			if($buffer == FALSE) {
				break;
			}
			
			$bufferSize = strlen($buffer);
			
			GtEnc_Dec($encType, $buffer, $bufferSize, $key, $keySize, ($rangeStart + $readSize) % $keySize);
			
			$readSize += $bufferSize;
			
			print($buffer);
		}
		
		fclose($file);
		
		ob_flush();
		flush();
	} else {
		return -2;
	}
	
	return $readSize;  // May be less than total file size because of multi-part transmission.
}

function GtEnc_DownloadFileEnc($sourceFileName, $fileName, $mimeType = "", $cacheEnable = 0, $cacheTimeSec = 0, $isAttachment = 1, $timeout = 0, $encType, $key, $keySize, $encNewType, $keyNew, $keyNewSize) {
	//Turn off output buffering to decrease cpu usage.
	ob_end_clean();
	
	// disable execution time limit
	set_time_limit($timeout);
			
	if(!is_readable($sourceFileName)) {
		return -1;
	}

	$fileSize = filesize($sourceFileName);
	
	$fileExtension = strtolower(substr(strrchr($fileName,"."), 1));
	$fileName = rawurldecode($fileName);

	/* Figure out the MIME type (if not specified) */
	$knownMimeTypes = array(
	"pdf" => "application/pdf",
	"txt" => "text/plain",
	"html" => "text/html",
	"htm" => "text/html",
	"exe" => "application/octet-stream",
	"zip" => "application/zip",
	"doc" => "application/msword",
	"xls" => "application/vnd.ms-excel",
	"ppt" => "application/vnd.ms-powerpoint",
	"gif" => "image/gif",
	"png" => "image/png",
	"jpeg"=> "image/jpg",
	"jpg" => "image/jpg",
	"php" => "text/plain",
	"mp3" => "audio/mpeg"  // For IE
	//"mp3" => "audio/mpeg, audio/x-mpeg, audio/x-mpeg-3, audio/mpeg3"
	);

	if($mimeType != "") {
		if(array_key_exists($fileExtension, $knownMimeTypes)) {
			$mimeType = $knownMimeTypes[$fileExtension];
		} else {
			$mimeType = "application/force-download";
		};
	};
	
	if ($cacheEnable == 1) {
		header("Cache-Control: private, max-age=$cacheTimeSec");
		//header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");
	} else {
		header('Cache-Control: public, must-revalidate, max-age=0');
    	header('Pragma: no-cache'); 
		header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");
	}

	// Required for IE, otherwise Content-Disposition may be ignored.
	if(ini_get('zlib.output_compression')) {
		ini_set('zlib.output_compression', 'Off');
	}

	header("Content-Transfer-Encoding: binary");
	header("Accept-Ranges: bytes");
	header("Content-Type: " . $mimeType);
	header("Content-Description: File Transfer");
	if($isAttachment == 1) {
		header('Content-Disposition: attachment; filename="'.$fileName.'"');
	} else {
		header('Content-Disposition: inline; filename="'.$fileName.'"');
	}
	
	
	// Multipart-download and download resuming support.
	$rangeStart = 0;
	if(isset($_SERVER['HTTP_RANGE'])) {
		fwrite($fp, $_SERVER['HTTP_RANGE']."\n");
		
		if(preg_match('/bytes=\h*(\d+)-(\d*)[\D.*]?/i', $_SERVER['HTTP_RANGE'], $matches))
		{
			$rangeStart = intval($matches[1]);
			if (!empty($matches[2]))
			{
				$rangeEnd	= intval($matches[2]);
			}
		}
		
		if(strlen($rangeStart) == 0) {
			$rangeStart = 0;	
		}
		
		if($rangeStart < 0) {
			$rangeStart = 0;	
		}
		
		if(strlen($rangeEnd) == 0) {
			$rangeEnd = $fileSize - 1;	
		}
		
		if($rangeEnd < 0) {
			$rangeEnd = 0;	
		}
		
		if($rangeEnd >= $fileSize) {
			$rangeEnd = $fileSize - 1;
		}
		
		if($rangeEnd < $rangeStart) {
			$rangeEnd = $rangeStart;	
		}

		$contentLength = $rangeEnd - $rangeStart + 1;
		
		header("HTTP/1.1 206 Partial Content");
		header("Content-Length: $contentLength");
		header("Content-Range: bytes ".$rangeStart."-".$rangeEnd."/".$fileSize);
	} else {
		$contentLength = $fileSize;
		header("Content-Length: $contentLength");
	}

	// Output the file itself.
	$chunkSize = 1048576; // 1MB buffer.
	$readSize = 0;
	if ($file = fopen($sourceFileName, 'rb')) {
		if($rangeStart > 0) {
			fseek($file, $rangeStart);
		}

		while (!feof($file) && (!connection_aborted()) && ($readSize < $contentLength)) {
			$buffer = fread($file, $chunkSize);
			if($buffer == FALSE) {
				break;
			}
			
			$bufferSize = strlen($buffer);
			
			GtEnc_DecThenEnc($encType, $buffer, $bufferSize, $key, $keySize, ($rangeStart + $readSize) % $keySize, $keyNew, $keyNewSize, ($rangeStart + $readSize) % $keyNewSize);
			
			$readSize += $bufferSize;
			
			print($buffer);
		}
		
		fclose($file);
		
		ob_flush();
		flush();
	} else {
		return -2;
	}
	
	return $readSize;  // May be less than total file size because of multi-part transmission.
}

function GtEnc_DownloadPhoto($sourceFileName, $fileName, $mimeType = "", $cacheEnable = 0, $cacheTimeSec = 0, $isAttachment = 1, $timeout = 0, $encType = 0, $key = "", $keySize = 0, $width = 0, $height = 0, $autoFit = 1, $fileExtensionSetting = "default", $quality = -1, $encode = 0) {
	//Turn off output buffering to decrease cpu usage.
	ob_end_clean();
	
	// Set execution time limit
	set_time_limit($timeout);
	
	if(!is_readable($sourceFileName)) {
		return -1;
	}

	$fileSize = filesize($sourceFileName);
	
	if ($fileExtensionSetting == "default") {
		$fileExtension = strtolower(substr(strrchr($fileName,"."), 1));
	} else {
		$fileExtension = $fileExtensionSetting;	
	}
	
	if ($fileExtension != "png" && $fileExtension != "jpg" && $fileExtension != "jpeg" && $fileExtension != "gif") {
		$fileExtension = "jpg";
		$encode = 1;
	}
	
	if ($fileExtension != strtolower(substr(strrchr($fileName,"."), 1))) {
		$encode = 1;
	}

	/* Figure out the MIME type (if not specified) */
	$knownMimeTypes = array(
		"png" => "image/png",
		"jpeg"=> "image/jpg",
		"jpg" => "image/jpg",
		"gif" => "image/gif"
	);

	if($mimeType == "") {
		if(array_key_exists($fileExtension, $knownMimeTypes)) {
			$mimeType = $knownMimeTypes[$fileExtension];
		} else {
			$mimeType = "image/jpg";
			$fileExtension = "jpg";
			$encode = 1;
		};
	};
	
	$baseame = basename($fileName, "." . $fileExtension);	
	$fileName = rawurldecode($baseame . "." . $fileExtension);
		
	if ($cacheEnable == 1) {
		header("Cache-Control: private, max-age=$cacheTimeSec");
		//header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");
	} else {
		header('Cache-Control: public, must-revalidate, max-age=0');
    	header('Pragma: no-cache'); 
		header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");
	}

	// Required for IE, otherwise Content-Disposition may be ignored.
	if(ini_get('zlib.output_compression')) {
		ini_set('zlib.output_compression', 'Off');
	}
	
	header("Content-Transfer-Encoding: binary");
	//header("Accept-Ranges: bytes");
	header("Content-Type: " . $mimeType);
	header("Content-Description: File Transfer");
	if($isAttachment == 1) {
		header('Content-Disposition: attachment; filename="'.$fileName.'"');
	} else {
		header('Content-Disposition: inline; filename="'.$fileName.'"');
	}

	$rangeStart = 0;	
	$contentLength = $fileSize;
	$rangeEnd = $fileSize - 1;
	$imageBuffer = NULL;

	// Output the file itself.
	$chunkSize = 1048576; // 1MB buffer.
	$readSize = 0;
	$file = fopen($sourceFileName, 'rb');
	if ($file != FALSE) {
		while(!feof($file) && (!connection_aborted()) && ($readSize < $contentLength)) {
			$buffer = fread($file, $chunkSize);
			if($buffer == FALSE) {
				break;
			}
			
			$bufferSize = strlen($buffer);
			
			GtEnc_Dec($encType, $buffer, $bufferSize, $key, $keySize, ($rangeStart + $readSize) % $keySize);
			
			$readSize += $bufferSize;
			
			$imageBuffer.=$buffer;
		}		
		
		fclose($file);		
		
		if($width > 0 || $height > 0) {
			$imageSrc = imagecreatefromstring($imageBuffer);
			
			$imageSrcWidth = imagesx($imageSrc);
			$imageSrcHeight = imagesy($imageSrc);
			
			if ($autoFit == 0 && $width > 0 && $height > 0) {
				$imageDestWidth = $width;
				$imageDestHeight = $height;
			} else {
				if($width > 0 && $height > 0) {
					if($width >= $height) {
						if($imageSrcWidth >= $imageSrcHeight) {
							$imageDestWidth = $imageSrcWidth * ($height / $imageSrcHeight);
							$imageDestHeight = $height;
						} else {
							$imageDestWidth = $width;
							$imageDestHeight = $imageSrcHeight * ($width / $imageSrcWidth);
						}
					} else {
						if($imageSrcWidth >= $imageSrcHeight) {
							$imageDestWidth = $width;
							$imageDestHeight = $imageSrcHeight * ($width / $imageSrcWidth);
						} else {
							$imageDestWidth = $imageSrcWidth * ($height / $imageSrcHeight);
							$imageDestHeight = $height;
						}
					}
				} else if($width > 0) {
						$imageDestWidth = $width;
						$imageDestHeight = $imageSrcHeight * ($width / $imageSrcWidth);
				} else if($height > 0) {
						$imageDestHeight = $height;
						$imageDestWidth = $imageSrcWidth * ($height / $imageSrcHeight);
				} else {
					$imageDestWidth = $imageSrcWidth;
					$imageDestHeight = $imageSrcHeight;
				}
			}
		}
		
		if ($imageDestWidth != $imageSrcWidth || $imageDestHeight != $imageSrcHeight) {
			$encode = 1;	
		}
		
		if ($quality != -1) {
			$encode = 1;	
		}
		
		if ($encode == 1) {
			$imageDest = imagecreatetruecolor($imageDestWidth, $imageDestHeight);
			imagecopyresampled($imageDest, $imageSrc, 0, 0 ,0, 0, $imageDestWidth, $imageDestHeight, $imageSrcWidth, $imageSrcHeight);
			
			imagedestroy($imageSrc);
			
			if($mimeType == "image/jpg") {
				if($quality < 0 || $quality > 100) {
					imagejpeg($imageDest);
				} else {
					imagejpeg($imageDest, NULL, $quality);
				}
			} else if($mimeType == "image/png") {
				if($quality < 0 || $quality >= 10) {
					imagepng($imageDest);
				} else {
					imagepng($imageDest, NULL, $quality);
				}
			} else if($mimeType == "image/gif") {
				imagegif($imageDest);
			} else {
				if($quality < 0 || $quality > 100) {
					imagejpeg($imageDest);
				} else {
					imagejpeg($imageDest, NULL, $quality);
				}
			}
			
			imagedestroy($imageDest);
		} else {
			header("Content-Length: $contentLength");
			
			print($imageBuffer);
		}
		
		$imageBuffer = NULL;
		
		ob_flush();
		flush();
	} else {
		return -2;
	}

	return $readSize;
}

function GtEnc_Enc($encType, &$content, $contentSize, $key, $keySize, $keyShift) {
	if($encType == 0) {
		return 1;	
	}
    $ctr = $keyShift;
    for ($i = 0; $i < $contentSize; $i ++) {
        if ($ctr == $keySize) $ctr = 0;
		$content[$i] = (~$content[$i])^$key[$ctr];
        $ctr++;
    }
    return 1;
}

function GtEnc_EncFile($sourceFileName , $sourceFileSize, $encFileName, $encType, $key, $keySize) {
	if (!is_readable($sourceFileName)) {
		return -1;
	}
	
	if (is_readable($encFileName)) {
		return -2;
	}
	
	$fileSize = filesize($sourceFileName);
	if ($fileSize != $sourceFileSize) {
		unlink($sourceFileName);
		return -7;
	}
	
	$chunkSize = 1048576; // 1MB buffer.
	$fileSource = fopen($sourceFileName, "rb");
	if ($fileSource == FALSE) {
		return -3;	
	}
	
	$fileEnc = fopen($encFileName, "wb");
	if ($fileEnc == FALSE) {
		fclose($fileSource);
		unlink($fileSource);
		return -4;	
	}
	
	$readSize = 0;
	while (!feof($fileSource)) {
		$buffer = fread($fileSource, $chunkSize);		
		if($buffer == FALSE) {
			fclose($fileSource);
			fclose($fileEnc);
			unlink($fileSource);
			return -5;	
		}
		
		$bufferSize = strlen($buffer);
		
		GtEnc_Enc($encType, $buffer, strlen($buffer), $key, $keySize, $readSize % $keySize);
		
		if (fwrite($fileEnc, $buffer) == FALSE) {
			fclose($fileSource);
			fclose($fileEnc);
			unlink($fileSource);
			return -6;	
		}
		
		$readSize += $bufferSize;
	}

	fclose($fileSource);
	fclose($fileEnc);	
	unlink($sourceFileName);
	return 1;
}

function GtEnc_GetNewKey($encType) {
	if($encType == 0) {
		return "";	
	}
	
	return hash("sha256", rand().rand().rand().rand());
}

function GtEnc_GetRealKey($encType, $encKey) {
	if($encType == 0) {
		return "";	
	}
	
	global $serverEncRealKeyPrefix;
	
	return hash("sha256", $serverEncRealKeyPrefix.$encKey);
}

function GtFile_DownloadFile($sourceFileName, $fileName, $mimeType = "", $cacheEnable = 0, $cacheTimeSec = 0, $isAttachment = 1, $timeout = 0) {
	//Turn off output buffering to decrease cpu usage.
	ob_end_clean();
	
	// disable execution time limit
	set_time_limit($timeout);
			
	if(!is_readable($sourceFileName)) {
		return -1;
	}

	$fileSize = filesize($sourceFileName);
	
	$fileExtension = strtolower(substr(strrchr($fileName,"."), 1));
	$fileName = rawurldecode($fileName);

	/* Figure out the MIME type (if not specified) */
	$knownMimeTypes = array(
	"pdf" => "application/pdf",
	"txt" => "text/plain",
	"html" => "text/html",
	"htm" => "text/html",
	"exe" => "application/octet-stream",
	"zip" => "application/zip",
	"doc" => "application/msword",
	"xls" => "application/vnd.ms-excel",
	"ppt" => "application/vnd.ms-powerpoint",
	"gif" => "image/gif",
	"png" => "image/png",
	"jpeg"=> "image/jpg",
	"jpg" => "image/jpg",
	"php" => "text/plain",
	"mp4" => "video/mp4",
	"mp3" => "audio/mpeg"  // For IE
	//"mp3" => "audio/mpeg, audio/x-mpeg, audio/x-mpeg-3, audio/mpeg3"
	);

	if($mimeType == "") {
		if(array_key_exists($fileExtension, $knownMimeTypes)) {
			$mimeType = $knownMimeTypes[$fileExtension];
		} else {
			$mimeType = "application/force-download";
		};
	};
	
	if ($cacheEnable == 1) {
		header("Cache-Control: private, max-age=$cacheTimeSec");
		//header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");
	} else {
		header('Cache-Control: public, must-revalidate, max-age=0');
    	header('Pragma: no-cache'); 
		header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");
	}

	// Required for IE, otherwise Content-Disposition may be ignored.
	if(ini_get('zlib.output_compression')) {
		ini_set('zlib.output_compression', 'Off');
	}

	header("Content-Transfer-Encoding: binary");
	header("Accept-Ranges: bytes");
	header("Content-Type: " . $mimeType);
	header("Content-Description: File Transfer");
	if($isAttachment == 1) {
		header('Content-Disposition: attachment; filename="'.$fileName.'"');
	} else {
		header('Content-Disposition: inline; filename="'.$fileName.'"');
	}
	
	
	// Multipart-download and download resuming support.
	$rangeStart = 0;
	if(isset($_SERVER['HTTP_RANGE'])) {
		fwrite($fp, $_SERVER['HTTP_RANGE']."\n");
		
		if (preg_match('/bytes=\h*(\d+)-(\d*)[\D.*]?/i', $_SERVER['HTTP_RANGE'], $matches))
		{
			$rangeStart = intval($matches[1]);
			if (!empty($matches[2]))
			{
				$rangeEnd = intval($matches[2]);
			}
		}
		
		if(strlen($rangeStart) == 0) {
			$rangeStart = 0;	
		}
		
		if($rangeStart < 0) {
			$rangeStart = 0;	
		}
		
		if(strlen($rangeEnd) == 0) {
			$rangeEnd = $fileSize - 1;	
		}
		
		if($rangeEnd < 0) {
			$rangeEnd = 0;	
		}
		
		if($rangeEnd >= $fileSize) {
			$rangeEnd = $fileSize - 1;
		}
		
		if($rangeEnd < $rangeStart) {
			$rangeEnd = $rangeStart;	
		}

		$contentLength = $rangeEnd - $rangeStart + 1;
		
		header("HTTP/1.1 206 Partial Content");
		header("Content-Length: $contentLength");
		header("Content-Range: bytes ".$rangeStart."-".$rangeEnd."/".$fileSize);
	} else {
		$contentLength = $fileSize;
		header("Content-Length: $contentLength");
	}

	// Output the file itself.
	$chunkSize = 1048576; // 1MB buffer.
	$readSize = 0;
	if ($file = fopen($sourceFileName, 'rb')) {
		if($rangeStart > 0) {
			fseek($file, $rangeStart);
		}

		while(!feof($file) && (!connection_aborted()) && ($readSize < $contentLength)) {
			$buffer = fread($file, $chunkSize);
			if($buffer == FALSE) {
				break;
			}
			
			$bufferSize = strlen($buffer);
						
			$readSize += $bufferSize;
			
			print($buffer);
		}
		
		fclose($file);
		
		ob_flush();
		flush();
	} else {
		return -2;
	}
	
	return $readSize;  // May be less than total file size because of multi-part transmission.
}

function GtHtml_ClearHtml($content) {
	$content = preg_replace("/&lt;a[^&gt;]*&gt;&lt;/i", "", $content);
	$content = preg_replace("/&lt;\/a&gt;/i", "", $content);
	$content = preg_replace("/&lt;div[^&gt;]*&gt;/i", "", $content);
	$content = preg_replace("/&lt;\/div&gt;/i", "", $content);
	$content = preg_replace("/&lt;font[^&gt;]*&gt;/i", "", $content);
	$content = preg_replace("/&lt;\/font&gt;/i", "", $content);
	$content = preg_replace("/&lt;span[^&gt;]*&gt;/i", "", $content);
	$content = preg_replace("/&lt;\/span&gt;/i", "", $content);
	$content = preg_replace("/&lt;p[^&gt;]*&gt;/i", "", $content);
	$content = preg_replace("/&lt;\/p&gt;/i", "", $content);
	$content = preg_replace("/&lt;!--[^&gt;]*--&gt;/i", "", $content);//注释内容
	$content = preg_replace("/style=.+?['|\"]/i",'',$content);//去除样式
	$content = preg_replace("/class=.+?['|\"]/i",'',$content);//去除样式
	$content = preg_replace("/id=.+?['|\"]/i",'',$content);//去除样式
	$content = preg_replace("/lang=.+?['|\"]/i",'',$content);//去除样式
	$content = preg_replace("/width=.+?['|\"]/i",'',$content);//去除样式
	$content = preg_replace("/height=.+?['|\"]/i",'',$content);//去除样式
	$content = preg_replace("/border=.+?['|\"]/i",'',$content);//去除样式
	$content = preg_replace("/face=.+?['|\"]/i",'',$content);//去除样式
	$content = preg_replace("/face=.+?['|\"]/",'',$content);//去除样式 只允许小写 正则匹配没有带 i 参数
	return $content;
}

function GtHttp_Get($url, array $get = NULL, array $options = array()) 
{    
    $defaults = array( 
        CURLOPT_URL => $url . (strpos($url, '?') === FALSE ? '?' : '') . http_build_query($get), 
        CURLOPT_HEADER => 0, 
        CURLOPT_RETURNTRANSFER => TRUE, 
        CURLOPT_TIMEOUT => 20 
    ); 
	
    $ch = curl_init();
    curl_setopt_array($ch, ($options + $defaults)); 
    if( ! $result = curl_exec($ch)) { 
        //trigger_error(curl_error($ch));
		curl_close($ch); 
		return curl_error($ch);
    } 
    curl_close($ch);
    return $result; 
}

function GtHttp_Post($url, array $post = NULL, array $options = array()) { 
    $defaults = array( 
        CURLOPT_POST => 1, 
        CURLOPT_HEADER => 0, 
        CURLOPT_URL => $url,
        CURLOPT_FRESH_CONNECT => 1, 
        CURLOPT_RETURNTRANSFER => 1, 
        CURLOPT_FORBID_REUSE => 1, 
        CURLOPT_TIMEOUT => 20, 
        CURLOPT_POSTFIELDS => http_build_query($post) 
    ); 

    $ch = curl_init(); 
    curl_setopt_array($ch, ($options + $defaults)); 
    if( ! $result = curl_exec($ch)) { 
        //trigger_error(curl_error($ch));
		curl_close($ch); 
		return curl_error($ch);
    } 
    curl_close($ch); 
    return $result; 
}

function GtInput_Secure($str) {
	$str = htmlspecialchars(stripslashes($str), ENT_QUOTES, "UTF-8");
	$str = mysql_real_escape_string($str);
	$str = mysql_escape_string($str);
	$str = str_ireplace("script", "blocked", $str);
	$str = str_ireplace("sleep", "blocked", $str);
	$str = str_ireplace("delay", "blocked", $str);
	return $str;
}

function GtInput_SecureForArticle($str) {
	$str = htmlspecialchars(stripslashes($str), ENT_QUOTES, "UTF-8");
	// $str = mysql_real_escape_string($str); \r\n problem
	$str = mysql_escape_string($str);
	return $str;
}

function GtInput_SecureForUrl($str) {
	$str = mysql_real_escape_string($str);
	$str = mysql_escape_string($str);
	$str = str_ireplace("script", "blocked", $str);
	$str = str_ireplace("sleep", "blocked", $str);
	$str = str_ireplace("delay", "blocked", $str);
	return $str;
}

function GtKeyCode_Add($keyCode, $keyHint, $keyName, $keyUrl, $keyControl = 0, $keyBlank = 0) {
	global $keyCodes;
	global $keyHints;
	global $keyNames;
	global $keyUrls;
	global $keyControls;
	global $keyBlanks;
	
	$keyCodes[]    = $keyCode;
	$keyHints[]    = $keyHint;
	$keyNames[]    = $keyName;
	$keyUrls[]     = $keyUrl;
	$keyControls[] = $keyControl;
	$keyBlanks[]   = $keyBlank;
}

function GtKeyword_Add($accountSn, $keyword) {
	global $dbLink;
	$timeSec = GtTime_GetGmtTimeSec();
	$sql = "INSERT INTO keywords (keyword, createAccountSn, createTimeSec) values('$keyword', '$accountSn', '$timeSec')";
	$result = mysql_query($sql, $dbLink);
	if($result == FALSE) {
		return 0;
	}
		
	return 1;
}

function GtLdif_IsOk($ldif) {
	if(GtStr_StartWith($ldif, "OK") == 0) {
		return -1;
	}
	
	if(GtStr_EndWith($ldif, ";;") == 0) {
		return -2;
	}
	
	return 1;
}

function GtLdif_EncodeStr($str) {
	if(strlen($str) == 0) {
		return "%00"; 
	}

	$str = str_ireplace("%", "%25", $str);
	$str = str_ireplace(";", "%3B", $str);
	return $str;
}

function GtLdif_DecodeStr($str) {
	if(strcmp($str, "%00") == 0) {
		return NULL;
	}

	$str = str_ireplace("%3B", ";", $str);
	$str = str_ireplace("%25", "%", $str);
	return $str;
}

function GtLog_A($type = 0, $accountSn = 0, $message = "", $url = "") {
	global $GT_LOG_LEVEL_ASSERT;
	return GtLog_Log($GT_LOG_LEVEL_ASSERT, $type, $accountSn, $message, $url);
}

function GtLog_D($type = 0, $accountSn = 0, $message = "", $url = "") {
	global $GT_LOG_LEVEL_DEBUG;
	return GtLog_Log($GT_LOG_LEVEL_DEBUG, $type, $accountSn, $message, $url);
}

function GtLog_E($type = 0, $accountSn = 0, $message = "", $url = "") {
	global $GT_LOG_LEVEL_ERROR;
	return GtLog_Log($GT_LOG_LEVEL_ERROR, $type, $accountSn, $message, $url);
}

function GtLog_I($type = 0, $accountSn = 0, $message = "", $url = "") {
	global $GT_LOG_LEVEL_INFO;
	return GtLog_Log($GT_LOG_LEVEL_INFO, $type, $accountSn, $message, $url);
}

function GtLog_Log($level = 0, $type = 0, $accountSn = 0, $message = "", $url = "") {
	global $dbLink;
	global $pagePath;
	
	$message = GtInput_Secure($message);
	
	$timeSec = GtTime_GetGmtTimeSec();
	//$timeDateTime = GtTime_GetDateTime2($timeSec);
	$fromIp = $_SERVER['REMOTE_ADDR'];
	
	$fromHost = ""; // For better performance.
	//$fromHost = gethostbyaddr($fromIp);
	//$fromHost = $_SERVER['REMOTE_HOST'];
	
	$fromAgent = GtInput_Secure($_SERVER['HTTP_USER_AGENT']);
	if ($url == "") {
		$url = "http://$_SERVER[HTTP_HOST]$_SERVER[REQUEST_URI]";
	};
	$url = GtInput_SecureForUrl($url);
	
	$sql = "INSERT INTO log (url, accountSn, message, fromIp, fromHost, fromAgent, timeSec, level, type) values('$url', '$accountSn', '$message', '$fromIp', '$fromHost', '$fromAgent', '$timeSec', '$level', '$type')";
	$result = mysql_query($sql, $dbLink);
	if($result == FALSE) {
		//echo "Log Error! ". $sql;
		//echo "Log Error! ". mysql_error();
		return 0;
	}

	return 1;
}

function GtLog_N($type = 0, $accountSn = 0, $message = "", $url = "") {
	global $GT_LOG_LEVEL_NONE;
	return GtLog_Log($GT_LOG_LEVEL_NONE, $type, $accountSn, $message, $url);
}

function GtLog_V($type = 0, $accountSn = 0, $message = "", $url = "") {
	global $GT_LOG_LEVEL_VERBOSE;
	return GtLog_Log($GT_LOG_LEVEL_VERBOSE, $type, $accountSn, $message, $url);
}

function GtLog_W($type = 0, $accountSn = 0, $message = "", $url = "") {
	global $GT_LOG_LEVEL_WARN;
	return GtLog_Log($GT_LOG_LEVEL_WARN, $type, $accountSn, $message, $url);
}

function GtStr_StartWith($str, $beginning, $caseInsensitivity = 1) {
	$ret = FALSE;
    if ($caseInsensitivity == 1) {
        $ret = (strncasecmp($str, $beginning, strlen($beginning)) == 0);
	} else {
        $ret = (strncmp($str, $beginning, strlen($beginning)) == 0);
	}
		
	if ($ret == FALSE) {
		return 0;	
	}
	
	return 1;
}

function GtStr_EndWith($str, $ending, $caseInsensitivity = 1) {
	$ret = FALSE;
    if ($caseInsensitivity == 1) {
        $ret = (strcasecmp(substr($str, strlen($haystack) - strlen($ending)), $str) == 0);
	} else {
        $ret = (strpos($str, $ending, strlen($haystack) - strlen($ending)) !== FALSE);
	}
	
	if ($ret == FALSE) {
		return 0;	
	}
	
	return 1;
}

function GtTime_ConvertToLocalTimeSec($timeSecGmt) {
	global $serverTimeZone;
	
	$serverTimeZoneShift = $serverTimeZone * 3600;
	
	return $timeSecGmt + $serverTimeZoneShift;  	
}

function GtTime_GetDateTime($timeSec) {
	return date("YmdHis", $timeSec);
}

function GtTime_GetDateTime2($timeSec) {
	return date("Y-m-d H:i:s", $timeSec);
}

function GtTime_GetGmtDateTimeMicro() {
	global $serverTimeZone;
	
	$microtimes = explode(' ', microtime());
	$sec = $microtimes[1] - $serverTimeZone * 3600;
	
    return date("YmdHis", $sec).substr($microtimes[0], 2, 6);
}

function GtTime_GetGmtTimeSec() {
	global $serverTimeZone;
	
	$serverTimeZoneShift = $serverTimeZone * 3600;
		
	return time() - $serverTimeZoneShift;  	
}

function GtTime_GetGmtTimeSecMicro() {
	global $serverTimeZone;
	
	$microtimes = explode(' ', microtime());
	$sec = $microtimes[1] - $serverTimeZone * 3600;
	
    return $sec.substr($microtimes[0], 1, 7);
}

function GtWhiteList_Check($ip) {
	global $dbLink;
	
	$sql = "SELECT sn FROM whiteList WHERE ip = '$ip'";
	$result = mysql_query($sql, $dbLink);
	$row = mysql_fetch_array($result);
	if($row == FALSE) {
		return 0;
	}
	
	return 1;
}











// Translation.


function t($id, $lang = "unknown", $page = "/") {
	global $pageLang;
	global $dbLink;
	
	$trans = "";
	
	if($lang == "unknown") {
		$lang = $pageLang;
	}
	
	/*
	$sql = "SELECT * FROM translation WHERE id = '$id' AND page = '$page'";
	$result = mysql_query($sql, $dbLink);
	$row = mysql_fetch_array($result);
	if($row == FALSE) {
		return $id;
	}
	
	if($lang == "zh_TW") {
		return $row['zh_TW'];
	} else if($lang == "zh_CN") {
		return $row['zh_CN'];
	} else {
		//return $row['en_US'];
	}
	*/
	
	if($lang == "zh_TW") {
		$trans = GtChinese_ToTC($id);
	} else if($lang == "zh_CN") {
		$trans = GtChinese_ToSC($id);
	} else {
		$trans = GtChinese_ToTC($id);
	}
	
	if ($trans == "关斓光华") {
		$trans = "关于光华";	
	}
	
	return $trans;
}

// Functions --

?>