#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

/* Callback function
	First three arguments will be handled by libcurl.
	void *userp should be set with CURLOPT_WRITEDATA.
*/
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    return fwrite(contents, size, nmemb, (FILE *)userp);
}

/* Usage
	./input <day> <session_cookie>
	The resultant input file will be stored on ./<day>.input, e.g. 1.input.
*/
int main(int argc, char **argv) {
	if (argc != 3) {
		fprintf(stderr, "Usage: ./input <day> <session_cookie>\n");
		return 1;
	}

    CURL *curl = curl_easy_init();
    if (!curl)
		return 1;

	/* Note
		asprintf() is from the GNU C extensions library.
		This code should link correctly on all modern POSIX-compliant systems,
		as well as *BSD. If you use MSVC, write your own input parser.
	*/
	char *o_filename;
	if (0 > asprintf(&o_filename, "%s.input", argv[1])) {
		fprintf(stderr, "asprintf(): (most likely) allocation error)\n");
		return 1;
	}
	FILE *fp;
	fp = fopen(o_filename, "w");

	char *input_url;
	if (0 > asprintf(&input_url, "https://adventofcode.com/2025/day/%s/input",
		argv[1])) {
		fprintf(stderr, "asprintf(): (most likely) allocation error)\n");
		return 1;
	}
	printf("GET %s\n", input_url);

    curl_easy_setopt(curl, CURLOPT_URL, input_url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

	char *session_cookie;
	if (0 > asprintf(&session_cookie, "session=%s", argv[2])) {
		fprintf(stderr, "asprintf(): (most likely) allocation error)\n");
		return 1;
	}

    curl_easy_setopt(curl, CURLOPT_COOKIE, session_cookie);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK)
        fprintf(stderr, "Request failed: %s\n", curl_easy_strerror(res));

    curl_easy_cleanup(curl);

	printf("Input file stored at: %s\n", o_filename);
	free(o_filename);
	free(input_url);
	free(session_cookie);
	fclose(fp);
    return 0;
}
