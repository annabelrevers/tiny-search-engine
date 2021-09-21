# CS50 Lab 4 Crawler Implementation

## crawler
We implement the crawler module with several functions.

The `crawler()` function is called in main after the `seedURL`, `pageDirectory`, and `depth` parameters are validated. In `crawler()`, we create a `bag` to store webpages to be explored and a `hashtable` to hold urls we have already explored. More about these data structures can be found in `bag.h` and 'hashtable.h` which are found in the `libcs50` modle. After creating these, we create a webpage for our seedURL and add it to the bag and hashtable. We then enter a while loop that iterates as long as the bag is not empty. In the while loop, we find the urls from the seed page and create a new page for them. We then add them to the bag to be explored. The page for each url added to the bag is fetched, but we do not explore those pages for more urls unless their depth is less than our maxDepth. To perform the operations in this while loop, we use several functions from our `pagedir.h` module. One the while loop is complete, we delete our bag and hashtable (which is deleted with a simple function called `mydelete()` that free its items).

## pagedir
The `pagedir` module contains several functions used in crawler.

### isDir
`isDir()` is used to validate the `pageDirectory` parameter in the main() of the crawler module. It attempts to create a file in our directory by creating a filepath and using `fopen()`. If `fopen()` returns NULL, we know the directory does not exist or is not writable.

### pagefetcher
`pagefetcher()` is called in crawler() and utilizes the webpage_fetch function from `webpage.h` of the `libcs50` module. If it fails, it returns false which is then accounted for in crawler.

### pagescanner
`pagescanner()` is called in crawler() and runs a while loop to get all the urls from a webpage. It checks if the url it finds is internal and can be normalized, and inserts it into our hashtable if it has not already been explored. If the insert is sucessful, it creates a new webpage for the url and adds it to the bag so it can be explored later on in crawler(). If there are any issues, the checks ensure that the url and webpage will not be explored.

### pagesaver
`pagesaver()` creates a file for storing the information of a webpage. It puts the url on the first line of the file, its depth on the second, and its html in the rest of the file. 

## libcs50
`libcs50` hold previoulsy written code to run the crawler and pagedir modules. It includes the data structures `bag`, `set`, `hashtable`, `webage`, and more. These data structures are used throughout our code as previously described. It also contains important files for memory allocation like `memory.h`, that are used by some of the data structures.

## Resource Managment
# Memory is dynamically allocated throughout the code with proper frees when necessary.



