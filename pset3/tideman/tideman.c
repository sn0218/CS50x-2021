#include <cs50.h>
#include <stdio.h>
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
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool make_cycle(int winner, int loser);

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
    // look for the candidate
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            // update the ranks array in descending order of preference
            // each ranks[i] stores the candidate's index
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        // loop starts from i+1 as preferences is incremented in this case where candidate[i] comes before candidate[j] in ranks
        for (int j = i + 1; j < candidate_count; j++)
        {
            (preferences[ranks[i]][ranks[j]])++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    pair_count = 0;
    // populate the preferences arrary to compare the value if i equals to j
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // initalize the array of strenght of victory
    int strength[pair_count];
    int temp;
    pair temp_pair;

    // assign the strenght element from preferences arrary and pairs array
    for (int i = 0; i < pair_count; i++)
    {
        strength[i] = preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner];
    }

    // selection sort to sort strenght and pairs in decreasing order
    for (int j = 0; j < pair_count; j++)
    {
        for (int k = j + 1; k < pair_count; k++)
        {
            if (strength[j] < strength[k])
            {
                temp = strength[j];
                strength[j] = strength[k];
                strength[k] = temp;

                temp_pair = pairs[j];
                pairs[j] = pairs[k];
                pairs[k] = temp_pair;
            }
        }
    }

    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // populate each sorted pair
    for (int i = 0; i < pair_count; i++)
    {
        int winner = pairs[i].winner;
        int loser = pairs[i].loser;

        // check if the current sorted pair creates cycle
        if (make_cycle(winner, loser) == false)
        {
            // if the current pair doesn't create cycle, make it true!
            locked[winner][loser] = true;
        }

    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    int false_counter = 0;
    
    
    for (int i = 0; i < candidate_count; i++)
    {
        
        for (int j = 0; j < candidate_count; j++)
        {
            // iterate over column by column
            // if there is no true (i.e. all false) in one column, the column index is the source 
            if ((locked[j][i]) == false)
            {
                false_counter++;
            }

        }
         
        if (false_counter == candidate_count)
        {
            printf("%s\n", candidates[i]);
        }
        
        false_counter = 0;
        
    }
   
    return;
}

// check the sorted pair if it create cycle
bool make_cycle(int winner, int loser)
{
    // base case
    // break the recursion if the winner in the existing locked pair is equal to the loser
    if (winner == loser)
    {
        return true;
    }

    // iterate over all candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // iterate over all candidate to find the candidiate who before the current winner in the locked pair
        if (locked[i][winner])
        {
            // recur to trace back the previous winner
            // the loser is remembered in the recursion
            return make_cycle(i, loser);
        }
    }

    return false;
}
