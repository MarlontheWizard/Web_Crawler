/*
--------------------------------
|          Web Crawler         |
|       Operating Systems      |
|                              |
|@Author: Marlon Dominguez     |
|@Author:                      |
|@Author:                      |
--------------------------------

@Functionality:

    • Multithreading: Use multiple threads to fetch web pages concurrently. 

    • URL Queue: Implement a thread-safe queue to manage URLs that are pending to be 
                 fetched. 

    • HTML Parsing: Extract links from the fetched web pages to find new URLs to crawl. 

    • Depth Control: Allow the crawler to limit the depth of the crawl to prevent infinite 
                     recursion. 
    
    • Synchronization: Implement synchronization mechanisms to manage access to 
                       shared resources among threads. 
    
    • Error Handling: Handle possible errors gracefully, including network errors, 
                      parsing errors, and dead links. 

    • Logging: Log the crawler’s activity, including fetched URLs and encountered errors.


@Requirements:

    • Makefile

    • GCC Compatible

    • README.txt


@Approach:
    1) URL & Parsing Data 
       ------------------
        C does not provide a native way to open websites or invoke HTTP requests.
        Why do we need HTTP requests? It is because we must aquire the HTML data from the website. 
        Therefore, we will be implementing the libcurl library (DOES NOT PROVIDE LOCKING).
            - On linux systems use [sudo apt install curl] to install libcurl. 
            - In the program, include the following header -> #include <curl/curl.h> 
        
        If still confused, please glance over the documentation -> https://everything.curl.dev/examples/get


    2) 

*/


#include <curl/curl.h> 
#include <cjson/cJSON.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

/*
void parse_html(){

}



int retrieve_html(){

}
*/
/*
Please read https://curl.se/libcurl/c/CURLOPT_WRITEFUNCTION.html#:~:text=This%20callback%20function%20gets%20called,nmemb%3B%20size%20is%20always%201.
for a better understanding of the write_callback function and it's arguments. 

In a brief description, the write_callback() function is responsible for the transfer of data. In this case,
between the webpage and the caller who is curl. The data being handled in this case is HTML, which comes 
in the form of strings. We have to implement this method ourselves. 

@param *ptr: ptr to curl handler. 
@param size_t size: size of data object being transferred. (e.g int = 4 bytes, char = 1 byte)
@param size_t nmemb: data object total count. In this case, think of chars or strings. 
@param void *userdata: Data container to be referenced. 
*/

int write_callback(char *ptr, size_t size, size_t nmemb, void *userdata){
    return fwrite(ptr, size, nmemb, stdout);
}

/*
The open_url() function takes a string url as input and creates an http
request to the respective page using libcurl. 

@param char *url: pointer to string, the unique resource identifier of the target page. 
*/
int open_url(char *url){
    
    /*
    Create libcurl object, or handler, necessary for http interaction. 
    The call to curl_easy_init() initializes the handler. 
    IMPORTANT: Make sure to close the handler when finished. 
    */
    CURL *curl_handler = curl_easy_init();

    //Check if the handler was initialized currectly, dereferencing the handler pointer gives us the error status. 
    if(curl_handler){

        /*
        In libcurl a variety of options exist to modify the behaviour of the curl handler. 
        These options are initialized using curl_set_opt(curl handler, option, option_parameter).
        
        What do we want the curl handler to do?
            -> We want it to establish connection with a web page 
            
            -> Retrieve HTML data 

        We must open the url before transferring the data. 
        */
        
        curl_easy_setopt(curl_handler, CURLOPT_URL, url);
        curl_easy_setopt(curl_handler, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl_handler, CURLOPT_USERAGENT, "libcurl-agent/1.0"); //Additional information for server requests

        //Execute the behaviour attributed to curl_handler.
        CURLcode flag = curl_easy_perform(curl_handler);

        if (flag != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(flag));
            return 1;
        }

        // Clean up
        curl_easy_cleanup(curl_handler);
        return 0; 
    }

    return 1; 
    
}


/*
The execute_page() function is the function we should call to process a web page, or url, within our crawler. 
Since C does not provide native support for retrieving web pages the process consists of multiple
steps. This function will begin the execution of those steps. 

@param char *url 
@return bool: returns false if error, true otherwise. 
*/
bool execute_page(char *url){
    
    //Open the URL
    if(open_url(url)){ //URL opened successfully, retrieve the html data

        return true; 
    }

    //URL not opened successfully
    return false; 
}


int main(){
    
    char *url = "https://www.ubisoft.com/en-us/";
    if(execute_page(url)){
        printf("Page executed successfully.");
    }

    else{
        printf("Page executed unsuccessfully.");
    }

    return 0; 
}