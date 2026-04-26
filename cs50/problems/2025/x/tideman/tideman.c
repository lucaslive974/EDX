#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool dfs_cycle(int head, int next);
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
int found_candidate_idx(string name);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    int idx = found_candidate_idx(name);

    if(idx == -1) return false;

    ranks[rank] = idx;
    return true;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for(int i = 0; i < candidate_count - 1; i++) {
        for(int j = i + 1; j < candidate_count; j++) {
            int winner_idx = ranks[i];
            int loser_idx = ranks[j];
            preferences[winner_idx][loser_idx]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for(int i = 0; i < candidate_count - 1; i++) {
        for(int j = i + 1; j < candidate_count; j++) {
            pair p;
            int diff = preferences[i][j] - preferences[j][i];
            if(diff == 0) continue;
            if(diff > 0) {
                p.winner = i;
                p.loser = j;
            } else {
                p.winner = j;
                p.loser = i;
            }
            pairs[pair_count++] = p;
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
   for(int i = 0; i < pair_count - 1; i++) {
        int max = i;

        int i_winner = pairs[i].winner, i_loser = pairs[i].loser;
        int i_diff = preferences[i_winner][i_loser] - preferences[i_loser][i_winner];

        for(int j = i + 1; j < pair_count; j++) {

            int j_winner = pairs[j].winner, j_loser = pairs[j].loser;
            int j_diff = preferences[j_winner][j_loser] - preferences[j_loser][j_winner];

            if(j_diff > i_diff) {
                max = j;
            }
        }
        if(max != i) {
            pair tmp = pairs[max];
            pairs[max] = pairs[i];
            pairs[i] = tmp;
        }
    }
   return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for(int i = 0; i < pair_count; i++) {
        int winner = pairs[i].winner;
        int loser  = pairs[i].loser;

        if(dfs_cycle(winner, loser)) continue;

        locked[winner][loser] = true;
    }
    return;
}

bool dfs_cycle(int head, int next) {
    if(head == next) return 1;
    for(int i = 0; i < candidate_count; i++) {
        if(locked[next][i]) {
            if(dfs_cycle(head, i)) return 1;
        }
    }

    return 0;
}

// Print the winner of the election
void print_winner(void)
{
    int winner = -1;

    for(int i = 0; i < candidate_count; i++) {
        int sum = 0;
        for(int j = 0; j < candidate_count; j++) {
            sum += locked[j][i];
        }
        if(sum == 0) winner = i;
    }
    printf("%s\n", candidates[winner]);
    return;
}

int found_candidate_idx(string name) {
    for(int i = 0; i < candidate_count; i++) {
        if(strcmp(candidates[i], name) == 0) return i;
    }
    return -1;
}
