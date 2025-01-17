#include <stdio.h>     // For printf, perror
#include <stdlib.h>    // For exit
#include <unistd.h>    // For fork, execve, pid_t
#include <sys/types.h> // For pid_t

// Function to play the song using afplay (macOS)
void play_song(void)
{
    pid_t pid = fork();  // Create a new process

    if (pid < 0) {
        // Fork failed
        perror("fork failed");
        return;
    }
    else if (pid == 0) {
        // Child process: Replace the child process with afplay
        // Adjust the path to your audio file if needed
        char *args[] = {"/usr/bin/cvlc", "--quiet", "./background.mp3", NULL};
        execve(args[0], args, NULL);

        // If execve returns, an error occurred
        perror("execve error");
        exit(EXIT_FAILURE);
    }
    // Parent process: simply continues; no wait, so music plays in the background
}

int main(void)
{
    printf("Testing play_song function...\n");

    // Call the play_song function
    play_song();

    // Do other work here or just wait so we can hear the audio
    printf("Music should be playing now. Press Enter to exit.\n");
    getchar(); // Wait for user input to keep the program running
    
    printf("Exiting program.\n");
    return 0;
}