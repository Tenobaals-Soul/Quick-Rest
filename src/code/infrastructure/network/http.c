#include<infrastructure/network/http.h>
#include<infrastructure/datastructures/string_dict.h>
#include<infrastructure/mstring.h>
#include<time.h>
#include<unistd.h>
#include<stdlib.h>

const char* http_code_message(int code) {
    switch (code) {
    case 100:
        return "Continue";
    case 101:
        return "Switching Protocol";
    case 102:
        return "Processing";
    case 103:
        return "Early Hints";
    case 200:
        return "OK";
    case 201:
        return "Created";
    case 202:
        return "Accepted";
    case 203:
        return "Non-Authorised Information";
    case 204:
        return "No Content";
    case 205:
        return "Reset Content";
    case 206:
        return "Partial Content";
    case 208:
        return "Already Reported";
    case 226:
        return "IM Used";
    case 300:
        return "Multiple Choices";
    case 301:
        return "Moved Permanently";
    case 302:
        return "Found";
    case 303:
        return "See Other";
    case 304:
        return "Not Modified";
    case 305:
        return "Use Proxy";
    case 306:
        return "unused";
    case 307:
        return "Temporary Redirekt";
    case 308:
        return "Permanent Redirect";
    case 400:
        return "Bad Request";
    case 401:
        return "Unauthorised";
    case 402:
        return "Payment Required";
    case 403:
        return "Forbidden";
    case 404:
        return "Not Found";
    case 405:
        return "Method Not Allowed";
    case 406:
        return "Not Acceptable";
    case 407:
        return "Proxy Authentification Required";
    case 408:
        return "Request Timeout";
    case 409:
        return "Conflict";
    case 410:
        return "Gone";
    case 411:
        return "Length Required";
    case 412:
        return "Precondition Failed";
    case 413:
        return "Payload Too Large";
    case 414:
        return "URI Too Long";
    case 415:
        return "Unsupported Media Type";
    case 416:
        return "Requested Range Not Satisfiable";
    case 417:
        return "Expectation Failed";
    case 421:
        return "Misdirected Result";
    case 426:
        return "Upgrade Required";
    case 428:
        return "Precondition Required";
    case 429:
        return "Too Many Requests";
    case 431:
        return "Request Header Fields Too Large";
    case 451:
        return "Unavailable For Legal Reasons";
    case 500:
        return "Internal Server Error";
    case 501:
        return "Not Implemented";
    case 502:
        return "Bad Gateway";
    case 503:
        return "Service Unavailible";
    case 504:
        return "Gateway Timeout";
    case 505:
        return "HTTP Version Not Supported";
    case 506:
        return "Variant Also Negotiates";
    case 507:
        return "Insufficient Storage";
    case 508:
        return "Loop Detected";
    case 510:
        return "Not Extended";
    case 511:
        return "Network Authentication Required";
    default:
        return "Internal Server Error";
    }
}

static const char* month_lookup[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
static const char* day_lookup[] = { "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };

struct http_time http_time() {
    struct http_time out;

    time(&out.rawtime);
    out.timeinfo = gmtime(&out.rawtime);
    sprintf(out.string, "%3s,%2d %3s %4d %2d:%2d:%2d GMT", day_lookup[out.timeinfo->tm_wday],
            out.timeinfo->tm_mday, month_lookup[out.timeinfo->tm_mon], (out.timeinfo->tm_year + 1900),
            out.timeinfo->tm_hour, out.timeinfo->tm_min, out.timeinfo->tm_sec);
    return out;
}

void http_abort(int fd, int code, const char* header, const char* message) {
    http_respond(fd, code, header, message);
    close(fd);
    exit(0);
}

void http_respond(int fd, int code, const char* header, const char* message) {
    const char* code_message = http_code_message(code);
    char* buffer = strmformat("HTTP/1.1 %3d %s\r\n%s\r\n%s\r\n",
        code, code_message, header ? header : "", message ? message : "");
    if (write(fd, buffer, strlen(buffer)) < 0) close(fd);
    free(buffer);
}

void http_abort_from_dict(int fd, int code, StringDict* header, const char* message) {
    http_respond_from_dict(fd, code, header, message);
    close(fd);
    exit(0);
}

static void append_string_dict_append_item(void* arg_env, const char* key, void* arg_val) {
    char* val = arg_val;
    char** env = arg_env;
    char* to_append = strmformat("%s: %s\r\n", key, val);
    *env = strmappend(*env, to_append);
    free(to_append);
}

void http_respond_from_dict(int fd, int code, StringDict* header, const char* message) {
    char* header_str = NULL;
    string_dict_complex_foreach(header, append_string_dict_append_item, &header_str);
    http_respond(fd, code, header_str, message);
    free(header_str);
}