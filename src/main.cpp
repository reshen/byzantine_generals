/* 
 * Entry point for byzantine generals coding sample.  This simple program
 * demonstrates the Byzantine General's Problem in which multiple generals 
 * of the Byzantine Empire's army need to coordinate to attack or retreat from 
 * a city and they must be able to detect traitors in their midst.  For a good 
 * synopsis of the problem, see Wikipedia 
 * [http://en.wikipedia.org/wiki/Byzantine_generals].
 *
 * Author: Reshen Amin <reshen@zensrc.com>
 *
 * Copyright 2009 Reshen Amin
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <iostream>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

using namespace std;

int main(int argc, char** argv)
{
    /* Safety check input */
    if (argc < 3)
    {
        cout << "Invalid usage." << endl;
        cout << "bgenerals.exe [total number of generals] [traitor generals]" << endl;
        return 1;
    }

    /* Use input from the console to determine:
     * - the number of generals (n)
     * - the number of traitor generals (t) 
     * 
     * If n >= (3t + 1), then the Generals will be able to 
     * solve the problem.
     */
    int totalNumGenerals = atoi(argv[1]);
    int traitorGenerals = atoi(argv[2]);
    cout << "Total number of generals (n): " << totalNumGenerals << endl;
    cout << "Total number of traitor generals (t): " << traitorGenerals << endl;

    cout << endl;

    if ((3 * traitorGenerals + 1) <= totalNumGenerals)
    {
        cout << "This problem is solvable." << endl;
    }
    else
    {
        cout << "This problem is NOT solvable." << endl << "We need at least (3t + 1) total generals, where t is the number of traitor generals in order to sove this problem!" << endl;
    }

    /* Spin off loyal general threads. */

    /* Spin off traitor general threads. */


    return 0;
}
