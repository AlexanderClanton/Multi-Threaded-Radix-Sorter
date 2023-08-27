#include <vector>

/*
 * Parallel Radix Sort with support for Most Significant Digit sorting only.
 *
 * If allowed to use ten cores or more, should sort a large set of randomly
 * selected unsigned integers apromixmately ten times faster than a single
 * threaded sort of the same random set.
 *
 * Whilst the choice of sorting algorithnm is a matter or personal choice,
 * a bucket sort where each bucket is sorted in a different thread will almost 
 * certainly deliver the best results.
 */
class RadixServer {
public:
    /*
     * Start a TCP/IP all address listener on PORT and wait for clients to send 
     * lists of unsiged integers to be sorted. When sorting, do not use more
     * that CORES cpu cores.
     */
    RadixServer(const int port, const unsigned int cores);
};

class RadixClient {
public:
    /*
     * Connect via TCP/IP to the server on HOSTNAME listening on PORT. Send
     * the unsigned integers in each list in LISTS to the server and wait
     * for the sorted lists to be returned.
     */
    void msd(const char *hostname, const int port, std::vector<std::reference_wrapper<std::vector<unsigned int>>> &lists);
};
