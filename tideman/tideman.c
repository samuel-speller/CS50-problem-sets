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
bool cycle(int cycle_end, int cycle_start);

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
    //iterate over the candidates to check if the name exists and then put the ranked name in the ranks array if it does
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    //two for loops to iterate over the 2 dimentional preferences array
    for (int i = 0; i < candidate_count; i++)
    {
        //only add to preferences when ranks[j] is highter than ranks[i]. e.g over each iteration of i there should be one less iteration over j. It might not work so test.
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]] = preferences[ranks[i]][ranks[j]] + 1;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;

                pair_count = pair_count + 1;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    //sort pairs via bubble sort

    int swap_counter = 1;

    while (swap_counter != 0)
    {
        swap_counter = 0;

        for (int i = 0; i < pair_count - 1; i++)
        {
            //find the winning margins from the preferences array
            int winning_margin_i = preferences[pairs[i].winner][pairs[i].loser]
                                   - preferences[pairs[i].loser][pairs[i].winner];

            int winning_margin_i_1 = preferences[pairs[i + 1].winner][pairs[i + 1].loser]
                                     - preferences[pairs[i + 1].loser][pairs[i + 1].winner];

            //we will sort the pairs array in decending order of winning margin
            if (winning_margin_i_1 > winning_margin_i)
            {
                //swap the pairs
                pair temp = pairs[i];
                pairs[i] = pairs[i + 1];
                pairs[i + 1] = temp;

                swap_counter = swap_counter + 1;
            }
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{

    /* FIRST ATTEMPT - based on  misundersyanding of the conditions for a loop


    for (int i = 0; i < pair_count - 1; i++)
    {
        //add an 'arrow' to the locked graph
        locked[pairs[i].winner][pairs[i].loser] = true;

        //check it hasn't made a cycle

        //int to keep track of if there is a true in a column of the locked array
        int true_count = 0;

        //int to keep track of if there is a line pointing to that candidate in the locked array
        int line_pointing = 0;

        for (int j = 0; j < candidate_count; j++)
        {
            for (int k = 0; k < candidate_count; k++)
            {
                if (locked[k][j])
                {
                    true_count = true_count + 1;
                }
            }

            if (true_count != 0)
            {
                line_pointing = line_pointing + 1;

                //reset true count
                true_count = 0;
            }
        }

        //check if there is a line pointing = candidate_count then there is an arrow pointing to every candidate (the graph is looped) so we have to delete the last step and break the loop

        if (line_pointing == candidate_count)
        {
            locked[pairs[i].winner][pairs[i].loser] = false;
        }
    }
    return;

    */

    //SECOND ATTEMPT

    //iterate over the pairs
    for (int i = 0; i < pair_count; i++)
    {
        //check to see if adding an arrow will create a cycle
        if (!cycle(pairs[i].loser, pairs[i].winner))
        {
            //add an 'arrow' to the locked graph
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }

    return;
}

// Print the winner of the election
void print_winner(void)
{
    //int to keep track of the amount of true values
    int true_count = 0;

    //iterate over the locked array, column by colum (notice locked[j][i] does this)
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])
            {
                true_count++;
            }
        }

        //if all the rows are false in a column e.g. no true, then that candidate has no arrows pointing to them
        if (true_count == 0)
        {
            printf("%s\n", candidates[i]);
        }

        //reset true_count before next pass
        true_count = 0;
    }

    return;
}

//function to check if a cycle is created using recursion
//iterate over the locked array to see if the end point is the same as the start point of the next candidate all the way round the graph (hard to explain!)
bool cycle(int cycle_end, int cycle_start)
{
    //return true if a cycle is created, RECURSION BASE CASE
    if (cycle_start == cycle_end)
    {
        return true;
    }

    //iterate over candidates
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[cycle_end][i])
        {
            if (cycle(i, cycle_start))
            {
                return true;
            }
        }
    }

    return false;

}