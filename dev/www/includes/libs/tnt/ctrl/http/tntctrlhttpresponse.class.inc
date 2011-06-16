<?php

///////////////////////////////////////////////////////////////////////////////
// TNT-SYNERGY: PHP Application Framework                            v2.0.00 //
// ======================================                                    //
//                                                                           //
// Copyright (c) 2003 by Nick Turner                                         //
// mail:info@tnt-synergy.com                                                 //
// http://www.tnt-synergy.com/                                               //
//                                                                           //
// TNT-SYNERGY is a PHP Application Framework. It encourages application     //
// structured application architectures based on the "Model2" version of the //
// Model View Controller (MVC) design paradigm. The framework supports the   //
// following key features:                                                   //
//     - Model                                                               //
//         - session persistance (PHP Sessions)                              //
//         - permanent persistance (Database library abstraction)            //
//         - authentication (Multi-user)                                     //
//         - authorization (Access control)                                  //
//         - application model (Object based view of web page)               //
//     - Control                                                             //
//        - state processing (Page states)                                   //
//        - event processing (User supplied events)                          //
//     - View                                                                //
//        - themes (application and/or user themes)                          //
//        - template based presentation (Template library abstraction)       //
//        - internationalisation (Multi-language support)                    //
//                                                                           //
// This program is free software. You can redistribute it and/or modify it   //
// under the terms of the GNU General Public License as published by the     //
// Free Software Foundation; either version 2 of the License, or (at your    //
// option) any later version.                                                //
//                                                                           //
//   #####################################################################   //
//   # This program is distributed in the hope that it will be useful,   #   //
//   # but WITHOUT ANY WARRANTY; without even the implied warranty of    #   //
//   # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU #   //
//   # General Public License for more details.                          #   //
//   #####################################################################   //
//                                                                           //
// TNT-Synergy is a wholly owned subsidiary of TurnerTronics. The project is //
// open development and is the culmination of many peoples time and effort.  //
// For a full list of developers and copyrights please refer to CREDITS.TXT  //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

/**
 * Ctrl Http Response Class
 * ~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * This class encapsulates HTTP response information.
 *
 * @package TNT.Ctrl.Http
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 */

/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTCtrlResponse');
TNTLoader::RequireClass('TNTCtrlRequest');
TNTLoader::RequireClass('TNTCtrlHttpRequest');

/*
 * Globals
 */

/*
 * Defines
 */

/**
 * Class to encapsulates HTTP response information.
 *
 * The methods and properties of this class are exposed through the
 * {@link TNTCtrlFront::response} property.
 *
 * It is also a singleton class and can be accessed via the static
 * method {@link TNTCtrlResponse::Singleton()}.
 *
 * @package TNT.Ctrl.Http
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 * @singleton
 */
class TNTCtrlHttpResponse extends TNTCtrlResponse {

    /**
     * The HTTP character set of the output stream.
     * @var     string                  HTTP character set code
     * @access  public
     */
    var $charset = null;

    /**
     * The HTTP MIME type of the output stream.
     * @var     string                  HTTP MIME type
     * @access  public
     */
    var $contentType = 'text/html';

    /**
     * The Platform for Internet Content Selection (PICS) rating.
     * @var     string                  PICs label
     * @access  public
     */
    var $picsLabel = null;

    /**
     * The Http Redirect Location.
     * @var     string                  redirect URL
     * @access  public
     */
    var $redirectURL = null;

    /**
     * Redirection should be done by 'javascript:window.location.replace' method
     * @var     boolean                 use URL replacement not HTTP redirect
     * @access  public
     */
    var $redirectReplace = false;

    /**
     * The HTTP status code.
     * @var     integer                 HTTP status code
     * @access  public
     */
    var $statusCode = 200;

    /**
     * The HTTP status description.
     * @var     string                  HTTP status description
     * @access  public
     */
    var $statusDesc = 'OK';

    /**
     * Add a HTTP header.
     *
     * The optional replace parameter indicates whether the header should
     * replace a previous similar header, or add a second header of the same
     * type. By default it will replace, but if you pass in FALSE as the third
     * argument you can force multiple headers of the same type.
     *
     * @access  public
     * @param   string      $name       name of HTTP header
     * @param   string      $value      value of HTTP header
     * @param   boolean     $replace    replace any existing header
     * @return  boolean                 true if successfull
     */
    function AddHeader($name, $value, $replace = true) {

        $ret = @header("$name: $value", $replace);

        return $ret;
    }

    /**
     * Add a Cookie
     *
     * The standard {@link SetVar()} method can be used to set simple
     * variables stored in cookies. However this method allows greater
     * control over the expiry of cookies etc.
     *
     * Add a cookie to be sent along with the rest of the HTTP headers. Like
     * other headers. 
     * 
     * All the arguments except the name argument are optional. If only the
     * name argument is present, the cookie by that name will be deleted from
     * the client. You may also replace any argument with NULL in order to skip
     * that argument. The expires argument is a regular Unix time integer as
     * returned by the PHP {@link time()} or {@link mktime()} functions. The
     * secure indicates that the cookie should only be transmitted over a
     * secure HTTPS connection. 
     *
     * Values other that strings are serialized into the cookie and then
     * desirialised in the {@link TNTCtrlHttpResponse}.
     *
     * If the cookie $value and $expires are passed as NULL then the code will
     * set an expiry date in the past to attempt to delete the cookie. For the
     * deletion to be successfull the $path, $domain and $secure should be the
     * same values as used to create the cookie initially.
     *
     * @access  public
     * @param   string      $name       name of cookie
     * @param   mixed       $value      value of cookie
     * @param   integer     $expires    expiration date and time (UTC)
     * @param   string      $path       virtual path
     * @param   string      $domain     name of the domain
     * @param   boolean     $secure     send only if secure connection
     * @return  boolean                 true if successfull
     */
    function AddCookie($name, $value = null, $expires = null, $path = null, $domain = null, $secure = null) {

        if (is_null($expires)) {
            $expires = (is_null($value) ? (time() - 86400) : 0);
        }
        if (is_null($path)) {
            $path = '';
        }
        if (is_null($domain)) {
            $domain = '';
        }
        if (is_null($secure)) {
            $secure = 0;
        }
        if (!is_null($value) && !is_scalar($value)) {
            $value = 'z:'.serialize($value);
        }

        $ret = setcookie($name, $value, $expires, $path, $domain, $secure);

        return $ret;
    }

    /**
     * Redirects a client to a new URL.
     *
     * This will set up the redirect headers.
     *
     * Any relative URL will be attempted to be converted to an absolute URL
     * if this fails then the method will return in error.
     
     * If '$end' is true then the method will call {@link Close()} which will
     * send the response followed by {@link TNT::Shutdown()} which will
     * terminate the script allowing the redirection to take place immediately.
     *
     * If '$replace' is specified then URL replacement is performed as opposed
     * to HTTP redirection. This means that no HTTP redirect header is sent,
     * instead a dummy page is returned to the browser containing a javascript
     * 'location.replace()' call. If javascript is not supported then a META
     * refresh tag is added as a backup. Failing that the dummy page contains
     * a link to click. The idea is that 'location.replace()' will not leave
     * a trace of this request in the clients history and not hence any client
     * 'Back' button can't return to this request.
     *
     * @access  public
     * @param   string      $url        target location
     * @param   boolean     $end        end current request
     * @param   boolean     $replace    do url replacement not redirection
     * @return  boolean                 true if successfull
     */
    function Redirect($url, $end = true, $replace = false) {

        $ret = false;

        TNTLoader::RequireClass('TNTUri');

        $uri =& new TNTUri($url);

        if (($ret = $uri->MakeAbsolute(null))) {
            $this->redirectURL = $uri->ToString();
        }

        destroy($uri);

        $this->redirectReplace = $replace;
 
        if ($end) {
            if ($ret) {
                $this->Close();
                TNT::Shutdown(0);
            } else {
                TNT::Abort("Failed to set redirection URL to '$url':", $uri->Error());
            }
        }

        return $ret;
    }

    /**
     * Adds a HTTP headers required to disable page caching.
     *
     * Disables caching of this page by
     *  + Setting an expires date in the past 
     *  + Setting last modfied date to now
     *  + Using HTTP/1.1 cache control
     *  + Using HTTP/1.0 cache control
     *
     * @access  public
     * @return  void
     */
    function NoCache() {

        $this->AddHeader('Expires', 'Mon, 26 Jul 1997 05:00:00 GMT');
        $this->AddHeader('Last-Modified', gmdate('D, d M Y H:i:s').' GMT'); 
        $this->AddHeader('Cache-Control', 'no-store, no-cache, must-revalidate');
        $this->AddHeader('Cache-Control', 'post-check=0, pre-check=0', false); 
        $this->AddHeader('Pragma', 'no-cache');

    }

    /**
     * Flushes all currently buffered output to the client.
     *
     * @access  public
     * @return  void
     */
    function Flush() {

        ob_flush();
        flush();

    }

    /**
     * Outputs data to the HTTP output stream.
     *
     * Supports the writing of TNTObjects by automatically calling the
     * {@link TNTObject::ToHTML()} method.
     *
     * The standard PHP functions {@link echo()}, {@link print()},
     * {@link printf()} can also be used to write to the output stream.
     *
     * @access  public
     * @param   mixed           $data   data to output
     * @return  void
     */
    function Output($data) {

        if (is_object($data) && implements($data, 'tntobject')) {
            $data = $data->ToHTML();
        }

        if (!is_scalar($data)) {
            $data = var_info($data);
        }

        echo $data;
        
    }

    /**
     * Adds an alert to the output stream.
     *
     * Alert messages are displayed in order by the next html page that is
     * displayed. Once the message is displayed it is cleared.
     *
     * An alert message can be sent along with a redirect and the alert will
     * be displayed by the next displayed page.
     *
     * Normally alert messages appear as popup message boxes but if the browser
     * doesn't support javascript then the alert will be displayed as a list
     * of messages enclosed in <div class="alert"></div> immediately before the
     * </body> tag. To change the position of the displayed alert message
     * add the following comment to the HTML and it will be replaced by the
     * alert div "<!-- ALERT -->".
     *
     * Each alert message is popuped or displayed separately.
     *
     * Passing NULL will clear all alerts !!!
     *
     * @access  public
     * @param   string          $msg    message to add (or null)
     * @return  void
     */
    function Alert($msg) {
        parent::Alert($msg);
    }

    /**
     * Close the response and send to the client.
     *
     * Once closed the VarMap becomes readOnly.
     *
     * @access  public
     * @return  void
     */
    function Close() {

        if ($this->isClientConnected) {

            $headers = array();
            $contents = null;

            // Filters can add variables, headers and cookies, but once
            // the output is filtered then the variables become read only.

            if ($this->redirectReplace) {
                // Redirect replacement will suppress any output

                $url1 = strtr($this->redirectUrl, array('\\'=>'\\\\',"'"=>"\\'",'"'=>'\\"',"\r"=>'\\r',"\n"=>'\\n'));

                $url2 = str_replace('"', '\"', $this->redirectUrl);

                $content = <<<EOS
<html>
<head>
<script language="javascript">
window.location.replace('$url1');
</script>
<noscript>
<meta type="refresh" content="0:$url2" />
</noscript>
<head>
<body>
<p>Click <a href="$url2">here</a> to continue</p>
</body>
</html>
EOS;

            } else if (!$this->suppressContent) {
                $contents = ob_get_contents();
                if ($contents && $this->_filters) {
                    foreach ($this->_filters as $filter) {
                        $contents = call_user_func($filter, $contents);
                    }
                }
            }

            ob_end_clean();

            $this->isReadOnly = true;

            if ($this->_vars) {
                foreach ($this->_vars as $name => $value) {
                    $this->AddCookie($name, $value);
                }
            }
    
            if ($this->statusCode) {
                $headers[] =  "HTTP/1.1 {$this->statusCode} {$this->statusDesc}";
            }
            if ($this->charset) {
                $headers[] = "Charset: {$this->charset}";
            }
            if ($this->contentType) {
                $headers[] = "Content-Type: {$this->contentType}";
            }
            //$headers[] =  'Content-Length: '.strlen($contents);
            if ($this->picsLabel) {
                $headers[] = "PICS-Label: {$this->picsLabel}";
            }
            if ($this->redirectURL && !$this->redirectReplace) {
                $headers[] = "Location: {$this->redirectURL}";
            }

            foreach($headers as $header) {
                header($header);
            }

            if ($contents) {
                echo $contents;
            }

            $this->isClientConnected = false;

            flush();
        }
        
    }

    /**
     * Replace URL's in the content which start "/synergy/"
     *
     * @access  private
     * @param   array   $matches    array of matches
     * @return  string              html to insert
     */
    function __ExpandUri($matches) {

        if (!is_array($matches) || count($matches) != 2) {
            TNT::Abort('TNTCtrlHttpResponse::_ExpandUri() - invalid matches', $matches);
        }

        $this->_UriFilter_uri->Set($matches[1]);

        $this->_UriFilter_uri->Expand();

        $ret = $this->_UriFilter_uri->ToString();

        return $ret;
    }

    /** 
     * UriFilter
     *
     * Expands all virtual paths in the output to real ones 
     *
     * @access  public
     * @param   string      $output     output to filter
     * @return  string                  filtered output
     */
    function _UriFilter($output) {

        TNTLoader::RequireClass('TNTUri');
        $this->_UriFilter_uri =& new TNTUri();

        // Expand all virtual paths in the final content
        $ret = preg_replace_callback(
            '|(([a-zA-Z]*:)?\~/[a-zA-Z0-9_./]*)|si',
            array(&$this, '__ExpandUri'), $output);

        destroy($this->_UriFilter_uri);

        return $ret;
    }

    /** 
     * HeadFilter
     *
     * Gathers any '<head>' tags that occur in the document and places them in
     * a single tag after the '<html>' tag.
     *
     * This allows components to insert code into the head section of the
     * document just by enclosing the code with '<head>' tags.
     *
     * @access  public
     * @param   string      $output     output to filter
     * @return  string                  filtered output
     */
    function _HeadFilter($output) {

        $ret = $output;

        if (substr_count($ret, '<head>') > 1) {
            $head = null;
            $rest = null;
            if ($bits = preg_split('/(<head>.*)<\/head>/Usi', $ret, -1,
                PREG_SPLIT_DELIM_CAPTURE|PREG_SPLIT_NO_EMPTY)) {
                foreach ($bits as $bit) {
                    if (strncasecmp($bit, '<head>', 6) == 0) {
                        $head .= substr($bit, 6);
                    } else {
                        $rest .= $bit;
                    }
                }
            }

            $ret = preg_replace('/<html[^>]*>/', '$0<head>'.addcslashes($head, '\\$').'</head>', $rest);
        }

        return $ret;
    }

    /** 
     * AlertFilter
     *
     * Adds any alert messages to the output stream and then clears the alerts.
     *
     * Checks for the comment '<!-- ALERT -->' and substitutes it with the
     * alert display code. If the comment is not found then it added the code
     * immediately before the '</body>' tag.
     *
     * If there is no '<!-- ALERT -->' or '</body>' tags then the alert code
     * is not added and the alert not cleared, thus allowing the alert to
     * continue on until a page with these tags is output.
     *
     * @access  public
     * @param   string      $output     output to filter
     * @return  string                  filtered output
     */
    function _AlertFilter($output) {

        $alerts = (array)$this->GetVar('tnt_alerts');
        if (!$alerts) {
            $request =& TNTCtrlHttpRequest::Singleton();
            $alerts = (array)$request->GetVar('tnt_alerts');
        }

        if (!$alerts) {
            $this->SetVar('tnt_alerts', null);
            $ret = $output;
        } else {
            $alert_script = '';
            $alert_noscript = '';

            foreach ($alerts as $msg) {
                $code_msg = strtr($msg, array('\\'=>'\\\\',"'"=>"\\'",'"'=>'\\"',"\r"=>'\\r',"\n"=>'\\n'));
                $html_msg = htmlentities($msg);

                $alert_script .= "alert('{$code_msg}');\n";
                $alert_noscript .= "    <li>{$html_msg}</li>\n";
            }
            
            $alert_html = "
<script language=\"javascript\" type=\"text/javascript\" defer=\"defer\">
<!--
$alert_script
// -->
</script>
<noscript>
<div class=\"alert\">
  <ul>
$alert_noscript
  </ul>
</div>
</noscript>
";

            $replace = str_replace('$', '\\$', $alert_html);

            $ret = preg_replace('/<!--\s*ALERT\s*--\s*>/', $replace, $output);

            if ($ret === $output) {
                // Nothing altered - so check for body tag
                $ret = preg_replace('/<\s*\/body\s*>/', $replace.'$0', $output);
            }

            if ($ret !== $output) {
                // Something replaced - so clear the alert
                $this->SetVar('tnt_alerts', null);
            }
        }

        return $ret;
    }

    /**
     * Construct the object
     *
     * It adds the default URIFilter which expands virtual paths in the
     * output to absolute paths.
     *
     * @access  public
     * @return  void                constructor
     */
    function TNTCtrlHttpResponse() {

        TNTCtrlResponse::TNTCtrlResponse();

        // Discard any current output
        //@ob_end_clean();

        // Start buffering any new output
        ob_start();

        $this->AddFilter(array(&$this, '_UriFilter'));
        $this->AddFilter(array(&$this, '_HeadFilter'));
        $this->AddFilter(array(&$this, '_AlertFilter'));

        $this->isClientConnected = true;
    }


    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTCtrlHttpResponse() {
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
