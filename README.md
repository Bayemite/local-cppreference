# local-cppreference
An offline copy of the cppreference website from https://github.com/PeterFeicht/cppreference-doc, with some simple search functionality added.

Actual search is done using https://github.com/farzher/fuzzysort.

## Added Files
[search.html](search.html): The searchbar landing page. Steals some of the original cppreference styling, but stripped down.
- Press '/' to jump to the searchbar.
- Press enter when searchbar is focused to open the first result in a new tab.
- Tab to cycle through results.
- You can use the URL parameter 'q' for search queries.
Example: `search.html?q=your-query-here`

[search.js](search.js): The search logic.

[search.css](search.css): Some minimalist styling.

[gen-search-data.cpp](gen-search-data.cpp): Recurses through a specified directory to gather data as defined in search.js. At least C++17 needed (filesystem standard library).

[fuzzysort-2.0.4.min.js](fuzzysort-2.0.4.min.js): From https://github.com/farzher/fuzzysort.

## Updating
- Download the latest 'html-book' release from https://github.com/PeterFeicht/cppreference-doc and unpack into repository root.
- Compile and run 'gen-search-data.cpp'.
    - Use [html-book directory]/reference/en as the root directory for the command line argument.
    - Redirect to a file for convenience.
- Place output into the 'data' variable, an array at the bottom of the 'search.js' file.
