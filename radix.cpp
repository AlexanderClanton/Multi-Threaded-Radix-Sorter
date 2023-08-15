#include <iostream>
#include <cmath>
#include <thread>
#include <mutex>
#include <crypt.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "cracker.h"

// Forward declarations
static void crackO(Message& mgz, unsigned int startO, unsigned int endO);
static bool crackH(int n, char arr[], int len, int L, char arr2[], char arr4[]);

int main() {
    Message msg;
    char currentHostname[20];
    gethostname(currentHostname, 20);

    // Predefined hostnames
    char arrN[35] = "aclanton";
    char arrT[20] = "thor";
    char arrO[20] = "olaf";
    char arrNO[20] = "nogbad";
    char arrI[20] = "noggin";

    // Initialize UDP socket for multicast
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in server_addr;
    bzero((char*) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(get_multicast_port());

    if (bind(sockfd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0)
        exit(-1);

    struct ip_mreq multicastRequest;
    multicastRequest.imr_multiaddr.s_addr = get_multicast_address();
    multicastRequest.imr_interface.s_addr = htonl(INADDR_ANY);

    if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*)&multicastRequest, sizeof(multicastRequest)) < 0)
        exit(-1);

    // Receive messages until we get one from 'arrN'
    while (true) {
        recvfrom(sockfd, &msg, sizeof(msg), 0, NULL, 0);
        if (strcmp(msg.cruzid, arrN) == 0) {
            break;
        }
    }
    close(sockfd);

    // Convert values from network byte order
    msg.num_passwds = ntohl(msg.num_passwds);
    msg.port = ntohs(msg.port);

    unsigned int start, end;
    unsigned int remainder = msg.num_passwds - (msg.num_passwds / 4);

    // Assign work based on hostname
    if (strcmp(currentHostname, arrT) == 0) {
        if (msg.num_passwds < 4) {
            start = 0;
            end = msg.num_passwds;
        } else {
            start = 0;
            end = msg.num_passwds / 4;
        }
    } else if (strcmp(currentHostname, arrO) == 0) {
        start = msg.num_passwds / 4;
        end = start + msg.num_passwds / 4 + (remainder == 3);
    } else if (strcmp(currentHostname, arrNO) == 0) {
        start = msg.num_passwds / 2;
        end = start + msg.num_passwds / 4 + (remainder >= 2);
    } else if (strcmp(currentHostname, arrI) == 0) {
        start = 3 * msg.num_passwds / 4;
        end = msg.num_passwds + (remainder == 3) - 1 + (remainder == 2);
    }

    crackO(msg, start, end);
    Message Hold;

// Function to initialize the UDP socket for multicast
int initializeUDPSocket() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    int ttl = 1;
    if (setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl)) < 0) {
        exit(-1);
    }
    return sockfd;
}

void processMessage(Message& msg, const char* cruzid, unsigned int start, unsigned int end) {
    msg.num_passwds = start;
    msg.port = end;
    strcpy(msg.cruzid, cruzid);
    msg.cruzid[strlen(cruzid)] = '\0';
}

// Function to handle incoming messages for serv1
void handleIncomingMessage(int sockfd) {
    for (;;) {
        recvfrom(sockfd, &Hold, sizeof(Hold), 0, NULL, 0);

        if (strcmp(Hold.cruzid, arrO) == 0 && !serv2) {
            for (unsigned int a = Hold.num_passwds; a < Hold.port; a++) {
                strncpy(msg.passwds[a], Hold.passwds[a], 4);
                msg.passwds[a][4] = '\0';
            }
            std::cout << "got olaf" << std::endl;
            serv2 = true;
        } else if (strcmp(Hold.cruzid, arrNO) == 0 && !serv3) {
            for (unsigned int a = Hold.num_passwds; a < Hold.port; a++) {
                strncpy(msg.passwds[a], Hold.passwds[a], 4);
                msg.passwds[a][4] = '\0';
            }
            std::cout << "got nogbad" << std::endl;
            serv3 = true;
        } else if (strcmp(Hold.cruzid, arrI) == 0 && !serv4) {
            for (unsigned int a = Hold.num_passwds; a < Hold.port; a++) {
                strncpy(msg.passwds[a], Hold.passwds[a], 4);
                msg.passwds[a][4] = '\0';
            }
            std::cout << "got noggin" << std::endl;
            serv4 = true;
        }

        if (serv2 && serv3 && serv4) break;
    }
}

if (serv1) {
    int sockfd = initializeUDPSocket();
    handleIncomingMessage(sockfd);
    close(sockfd);
} else {
    struct sockaddr_in multicastAddr;
    memset(&multicastAddr, 0, sizeof(multicastAddr));
    multicastAddr.sin_family = AF_INET;
    multicastAddr.sin_addr.s_addr = get_multicast_address();
    multicastAddr.sin_port = htons(get_multicast_port());

    int sockfd = initializeUDPSocket();

    const char* servCruzid = serv2 ? "olaf" : serv3 ? "nogbad" : "noggin";
    processMessage(msg, servCruzid, start, end);

    unsigned int ti = 0;
    while (ti <= 75) {
        sendto(sockfd, &msg, sizeof(msg), 0, (struct sockaddr*) &multicastAddr, sizeof(multicastAddr));
        sleep(1);
        ti++;
    }

    close(sockfd);
}
// Check conditions for service availability
if (serv1 == true || serv4S == true) {
    
    // Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // Check if socket creation failed
    if (sockfd < 0) 
        exit(-1);
    
    // Get server details based on hostname
    struct hostent *server = gethostbyname(msg.hostname);
    // Check if the hostname is invalid
    if (server == NULL)
        exit(-1);

    // Define server address structure
    struct sockaddr_in serv_addr;
    // Clear server address structure
    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    // Copy address data
    bcopy((char*)server->h_addr, (char*)&serv_addr.sin_addr.s_addr, server->h_length);

    // Convert port number to network byte order
    msg.port = htons(msg.port);
    serv_addr.sin_port = msg.port;
    // Convert number of passwords to network byte order
    msg.num_passwds = htonl(msg.num_passwds);

    // Connect to the server
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
        exit(-1);

    // Send the message to the server
    write(sockfd, &msg, sizeof(msg));
    // Close the socket connection
    close(sockfd);
}
return 0;

// Function to crack passwords using multi-threading
void crackO(Message &mgz, unsigned int startO, unsigned int endO) {
    // Constants
    unsigned int totperm2 = 615680;
    unsigned int ex = 16;
    unsigned int startZ = 0;
    unsigned int endZ = totperm2 + ex;
    char arr2[14];
    bool truth = false;
    unsigned int x;
    unsigned int a;
    unsigned int count = 0;
    // Container for threads
    std::vector<std::thread> cont;

    // Lambda function for multi-threaded password cracking
    auto lambaC = [&arr2, &x, &a, &mgz, &truth](unsigned int start, unsigned int end) {
        unsigned int c = start;
        bool truthL = false;
        char arrF[5];
        // While we have not reached the end and have not found the password
        while (c < end && !truth) {
            truthL = crackH(c, mgz.alphabet, 62, 4, arr2, arrF);
            // If a password match is found
            if (truthL) {
                // Copy the found password segment
                for (unsigned int i = 0; i < 4; i++) {
                    mgz.passwds[x][i] = arrF[i];
                }
                truth = true;
                mgz.passwds[x][4] = '\0';
            }
            c += 1;
        }
    };
    
    // Iterate through the password range
    for (x = startO; x < endO; x++) {
        // Copy the current password to local array
        for (a = 0; a < 13; a++) {
            arr2[a] = mgz.passwds[x][a];
        } 
        // Create multiple threads for concurrent password cracking
        for (unsigned int f = 0; f < 24; f++) {
            std::thread t1(lambaC, startZ, endZ);
            cont.push_back(std::move(t1));
            // Adjust ranges
            if (count == 0) {
                startZ += endZ;
            } else {
                startZ += totperm2;
            }
            count += 1;
            endZ += totperm2;
        }
        // Wait for all threads to finish
        for (unsigned int mft = 0; mft < cont.size(); mft++) {
            cont.at(mft).join();
        }
        // Reset parameters for next iteration
        count = 0;
        truth = false;
        startZ = 0;
        endZ = totperm2 + ex;
        cont.clear();
    }
}


bool crackH(int n, char arr[], int len, int L, char arr2[], char arr4[]) {
    
    // Declare and initialize a temporary array and salt array for password manipulation
    char arr3[5];
    char salt[3];
    
    // Assign first two characters of arr2 to salt for hashing purposes
    salt[0] = arr2[0];
    salt[1] = arr2[1];
    
    // Convert integer 'n' into a character array based on the provided alphabet
    for (int x = 0; x < L; x++) {
        arr3[x] = arr[n % len];
        n /= len;
    }
    
    // Prepare crypt_data structure for hashing
    struct crypt_data data;
    data.initialized = 0;
    
    // Encrypt 'arr3' with the provided salt
    char *valz = crypt_r(arr3, salt, &data);
    
    // Compare the encrypted string with the original one in arr2
    if (strcmp(arr2, valz) == 0) {
        // If match found, copy the current arrangement to the output array 'arr4'
        for (unsigned int z = 0; z < 4; z++) {
            arr4[z] = arr3[z];
        }
        return true;
    } else {
        // If no match, return false
        return false;
    }
}