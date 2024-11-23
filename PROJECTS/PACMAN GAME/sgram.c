#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 100
#define MAX_NAME_LEN 50
#define MAX_POST_LEN 100

// Struct for a post
typedef struct Post {
    int postId;
    char content[MAX_POST_LEN];
    struct Post* next;
} Post;

// Struct for a friend
typedef struct Friend {
    int friendId;
    struct Friend* next;
} Friend;

// Struct for a user
typedef struct User {
    int userId;
    char name[MAX_NAME_LEN];
    Friend* friendsList;
    Post* posts;
} User;

// User database
User users[MAX_USERS];
int totalUsers = 0;

// Function to find user by ID
User* findUserById(int userId) {
    for (int i = 0; i < totalUsers; i++) {
        if (users[i].userId == userId) {
            return &users[i];
        }
    }
    return NULL;
}

// Add friend
void addFriend(int userId, int friendId) {
    User* user = findUserById(userId);
    User* friendUser = findUserById(friendId);

    if (user == NULL || friendUser == NULL) {
        printf("User(s) not found.\n");
        return;
    }

    // Check if already friends
    Friend* temp = user->friendsList;
    while (temp != NULL) {
        if (temp->friendId == friendId) {
            printf("%s and %s are already friends.\n", user->name, friendUser->name);
            return;
        }
        temp = temp->next;
    }

    // Add friend to user's friend list
    Friend* newFriend = (Friend*)malloc(sizeof(Friend));
    newFriend->friendId = friendId;
    newFriend->next = user->friendsList;
    user->friendsList = newFriend;

    // Add user to friend's friend list
    Friend* newUserFriend = (Friend*)malloc(sizeof(Friend));
    newUserFriend->friendId = userId;
    newUserFriend->next = friendUser->friendsList;
    friendUser->friendsList = newUserFriend;

    printf("%s and %s are now friends!\n", user->name, friendUser->name);
}

// Remove friend
void removeFriend(int userId, int friendId) {
    User* user = findUserById(userId);
    User* friendUser = findUserById(friendId);

    if (user == NULL || friendUser == NULL) {
        printf("User(s) not found.\n");
        return;
    }

    // Remove friend from user's friend list
    Friend **ptr = &user->friendsList;
    while (*ptr && (*ptr)->friendId != friendId) {
        ptr = &(*ptr)->next;
    }
    if (*ptr) {
        Friend* temp = *ptr;
        *ptr = (*ptr)->next;
        free(temp);
    }

    // Remove user from friend's friend list
    ptr = &friendUser->friendsList;
    while (*ptr && (*ptr)->friendId != userId) {
        ptr = &(*ptr)->next;
    }
    if (*ptr) {
        Friend* temp = *ptr;
        *ptr = (*ptr)->next;
        free(temp);
    }

    printf("%s and %s are no longer friends.\n", user->name, friendUser->name);
}

// Find mutual friends
void findMutualFriends(int userId1, int userId2) {
    User* user1 = findUserById(userId1);
    User* user2 = findUserById(userId2);

    if (user1 == NULL || user2 == NULL) {
        printf("User(s) not found.\n");
        return;
    }

    printf("Mutual friends of %s and %s:\n", user1->name, user2->name);
    Friend* temp1 = user1->friendsList;
    while (temp1 != NULL) {
        Friend* temp2 = user2->friendsList;
        while (temp2 != NULL) {
            if (temp1->friendId == temp2->friendId) {
                User* mutualFriend = findUserById(temp1->friendId);
                printf("%s\n", mutualFriend->name);
            }
            temp2 = temp2->next;
        }
        temp1 = temp1->next;
    }
}

// Add a post
void addPost(int userId, const char* content) {
    User* user = findUserById(userId);
    if (user == NULL) {
        printf("User not found.\n");
        return;
    }

    // Create a new post
    Post* newPost = (Post*)malloc(sizeof(Post));
    newPost->postId = rand() % 1000;  // Random post ID
    strcpy(newPost->content, content);
    newPost->next = user->posts;
    user->posts = newPost;

    printf("Post added by %s.\n", user->name);
}

// Delete a specific post
void deletePost(int userId, int postId) {
    User* user = findUserById(userId);
    if (user == NULL) {
        printf("User not found.\n");
        return;
    }

    Post** ptr = &user->posts;
    while (*ptr && (*ptr)->postId != postId) {
        ptr = &(*ptr)->next;
    }
    if (*ptr) {
        Post* temp = *ptr;
        *ptr = (*ptr)->next;
        free(temp);
        printf("Post deleted.\n");
    } else {
        printf("Post not found.\n");
    }
}

// View all posts of a user
void viewPosts(int userId) {
    User* user = findUserById(userId);
    if (user == NULL) {
        printf("User not found.\n");
        return;
    }

    printf("Posts by %s:\n", user->name);
    Post* temp = user->posts;
    while (temp != NULL) {
        printf("Post ID %d: %s\n", temp->postId, temp->content);
        temp = temp->next;
    }
}

// View friends list of a user
void viewFriends(int userId) {
    User* user = findUserById(userId);
    if (user == NULL) {
        printf("User not found.\n");
        return;
    }

    printf("Friends of %s:\n", user->name);
    Friend* temp = user->friendsList;
    while (temp != NULL) {
        User* friendUser = findUserById(temp->friendId);
        printf("%s\n", friendUser->name);
        temp = temp->next;
    }
}

// Register a new user
void registerUser(const char* name) {
    if (totalUsers >= MAX_USERS) {
        printf("User limit reached.\n");
        return;
    }

    users[totalUsers].userId = totalUsers + 1;
    strcpy(users[totalUsers].name, name);
    users[totalUsers].friendsList = NULL;
    users[totalUsers].posts = NULL;
    printf("User %s registered with User ID: %d\n", name, users[totalUsers].userId);
    totalUsers++;
}

int main() {
    int choice, userId1, userId2, postId;
    char name[MAX_NAME_LEN], content[MAX_POST_LEN];

    while (1) {
        printf("\n--- Social Media Platform ---\n");
        printf("1. Register User\n2. Add Friend\n3. Remove Friend\n4. Find Mutual Friends\n");
        printf("5. Add Post\n6. Delete Post\n7. View Posts\n8. View Friends List\n9. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter name: ");
                scanf(" %[^\n]%*c", name);
                registerUser(name);
                break;
            case 2:
                printf("Enter your User ID and Friend's User ID: ");
                scanf("%d %d", &userId1, &userId2);
                addFriend(userId1, userId2);
                break;
            case 3:
                printf("Enter your User ID and Friend's User ID: ");
                scanf("%d %d", &userId1, &userId2);
                removeFriend(userId1, userId2);
                break;
            case 4:
                printf("Enter User ID of two users: ");
                scanf("%d %d", &userId1, &userId2);
                findMutualFriends(userId1, userId2);
                break;
            case 5:
                printf("Enter User ID and post content: ");
                scanf("%d", &userId1);
                getchar(); // to consume newline
                fgets(content, MAX_POST_LEN, stdin);
                content[strcspn(content, "\n")] = '\0'; // Remove newline
                addPost(userId1, content);
                break;
            case 6:
                printf("Enter User ID and Post ID to delete: ");
                scanf("%d %d", &userId1, &postId);
                deletePost(userId1, postId);
                break;
            case 7:
                printf("Enter User ID to view posts: ");
                scanf("%d", &userId1);
                viewPosts(userId1);
                break;
            case 8:
                printf("Enter User ID to view friends list: ");
                scanf("%d", &userId1);
                viewFriends(userId1);
                break;
            case 9:
                exit(0);
            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}
