#ifndef __PROGTEST__
#include <cassert>
#include <iomanip>
#include <cstdint>
#include <iostream>
#include <memory>
#include <limits>
#include <optional>
#include <algorithm>
#include <bitset>
#include <list>
#include <array>
#include <vector>
#include <deque>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <random>
#include <type_traits>

using Price = unsigned long long;
using Employee = size_t;
inline constexpr Employee NO_EMPLOYEE = -1;
using Gift = size_t;

#endif

// using SIZE_MAX ( maximum of size_t)

 std::vector<Employee> findVerticesofBoss(Employee manager, const std::vector<std::vector<Employee>> &children)
{
    std::vector<Employee> result;
    std::stack< Employee> S;
    S.push( manager);
    while (!S.empty())
    {
        Employee currentEmployee = S.top();
        S.pop();
        result.push_back(currentEmployee);
        for (size_t i = 0; i < children[currentEmployee].size(); i++)
        {
            S.push(children[currentEmployee][i]);
        }
    }
    return result;
}

std::pair<size_t, std::vector<Employee>> topSortVertices(std::vector<Employee> &vertices, const std::vector<Employee> &boss)
{
    std::unordered_map<Employee, size_t> inDegrees;
    for (size_t i = 0; i < vertices.size(); i++)
    {
        if (boss[vertices[i]] != NO_EMPLOYEE)
        {
            if (inDegrees.count(boss[vertices[i]]) > 0)
                inDegrees[boss[vertices[i]]] += 1;
            else
                inDegrees[boss[vertices[i]]] = 1;
        }
        else
        {
            inDegrees[vertices[i]] = 0;
        }
    }

    std::queue<Employee> Q;
    size_t noOfLeaves = 0;
    for (size_t i = 0; i < vertices.size(); i++)
    {
        if (inDegrees[vertices[i]] == 0)
        {
            Q.push(vertices[i]);
            noOfLeaves++;
        }
    }

    std::vector<Employee> result;
    while (!Q.empty())
    {
        Employee currentEmployee = Q.front();
        Q.pop();
        result.push_back(currentEmployee);
        if (boss[currentEmployee] != NO_EMPLOYEE)
        {
            inDegrees[boss[currentEmployee]] -= 1;
            if (inDegrees[boss[currentEmployee]] == 0)
                Q.push(boss[currentEmployee]);
        }
    }

    return {noOfLeaves, result};
}

std::pair<Price, std::vector<Gift>> optimize_gifts(
    const std::vector<Employee> &boss,
    const std::vector<Price> &gift_price)
{

    std::vector<std::vector<Employee>> children(boss.size());
    std::vector<Employee> managers;
    for (size_t i = 0; i < boss.size(); ++i)
    {
        if (boss[i] != NO_EMPLOYEE)
            children[boss[i]].push_back(i);
        else
            managers.push_back(i);
    }

    std::vector<std::pair<Price, size_t>> pairArray(gift_price.size());
    for (size_t i = 0; i < gift_price.size(); i++)
    {
        pairArray[i] = {gift_price[i], i};
    }

    std::sort(pairArray.begin(), pairArray.end());

    std::vector<Gift> sortedGifts(gift_price.size());
    for (size_t i = 0; i < pairArray.size(); i++)
    {
        sortedGifts[i] = pairArray[i].second;
    }

    Price resultPrice = 0;
    std::vector<Gift> result (boss.size());
    std::vector<Gift> minimumPriceIndex(boss.size() , 0);
    std::vector<Gift> secondMinimumPriceIndex(boss.size() , 0);

    for (size_t i = 0; i < managers.size(); i++)
    {
        std::vector<Employee> verticesInTree = findVerticesofBoss(managers[i], children);;


        std::pair<size_t, std::vector<Employee>> topSortTree = topSortVertices(verticesInTree, boss);
        size_t noOfLeaves = topSortTree.first;
        std::vector<Employee> topSortedEmployee = topSortTree.second;
        std::unordered_map<Employee,size_t> topSortedEmployeevalue;
        for(size_t i = 0 ; i < topSortedEmployee.size() ; i++)
        {
            topSortedEmployeevalue[topSortedEmployee[i]] = i;
        } 


        size_t useDepth = (children[managers[i]].size() + 5) < gift_price.size() ? (children[managers[i]].size() + 5) : gift_price.size();
        std::vector<std::vector<Price>> totalPrice(topSortedEmployee.size(), std::vector<Price>(useDepth, 0));

        for (size_t i = 0; i < noOfLeaves; i++)
        {
            for (size_t j = 0; j < useDepth; j++)
            {
                totalPrice[i][j] = gift_price[sortedGifts[j]];
            }
            minimumPriceIndex[i] = 0;
            secondMinimumPriceIndex[i] = 1;
        }

        for (size_t i = noOfLeaves; i < topSortedEmployee.size(); i++)
        {   
            Price minPriceOfParent = SIZE_MAX;
            Price secondMinPriceOfParent = SIZE_MAX;
            minimumPriceIndex[i] = 0;
            secondMinimumPriceIndex[i] = 0;


            for (size_t j = 0; j < useDepth; j++)
            {   
                totalPrice[i][j] += gift_price[sortedGifts[j]];

                for (size_t k = 0; k < children[topSortedEmployee[i]].size(); k++)
                {   
                    Employee childEmployee = children[topSortedEmployee[i]][k];
                    size_t child = topSortedEmployeevalue[childEmployee];

                    if( j != minimumPriceIndex[child])
                    {
                        totalPrice[i][j] += totalPrice[child][minimumPriceIndex[child]];
                    }
                    else
                    {
                        totalPrice[i][j] += totalPrice[child] [secondMinimumPriceIndex[child]];
                    }


                }
                
                if(totalPrice[i][j] <= minPriceOfParent)
                {
                    secondMinimumPriceIndex[i] = minimumPriceIndex[i];
                    minimumPriceIndex[i] = j;
                    secondMinPriceOfParent = minPriceOfParent;
                    minPriceOfParent = totalPrice[i][j];
                }
                else if(totalPrice[i][j] <= secondMinPriceOfParent)
                {
                    secondMinPriceOfParent = totalPrice[i][j];
                    secondMinimumPriceIndex[i] = j;
                }

            }

\
        }


        Employee manager = topSortedEmployee.size()-1;
        Gift minGiftForManager = minimumPriceIndex[manager];
        Price minimum = totalPrice[manager][minGiftForManager];


        resultPrice += minimum;

        std::queue<std::pair<Gift, Employee>> Q;

        Q.push({minGiftForManager,manager });
        while (!Q.empty())
        {
            Employee currentEmployee = Q.front().second;
            Gift currentGift = Q.front().first;


            Q.pop();
            result[topSortedEmployee[currentEmployee]] = sortedGifts[currentGift];
            for(size_t i = 0 ; i < children[topSortedEmployee[currentEmployee]].size(); i++)
            {
                Employee child = children[topSortedEmployee[currentEmployee]][i];
                if(currentGift != minimumPriceIndex[topSortedEmployeevalue[child]])
                {
                    Q.push({minimumPriceIndex[topSortedEmployeevalue[child]],topSortedEmployeevalue[child]});
                }
                else
                {
                    Q.push({secondMinimumPriceIndex[topSortedEmployeevalue[child]],topSortedEmployeevalue[child]});

                }
            }
            
        }

    }





    return std::make_pair(resultPrice,result);
}

#ifndef __PROGTEST__

const std::tuple<Price, std::vector<Employee>, std::vector<Price>> EXAMPLES[] = {
    {12, {2, 2, 4, 4, NO_EMPLOYEE, NO_EMPLOYEE, 5, 5, 5}, {4, 6, 8, 3, 1, 2}},
      { 11, {2,2,4,4,NO_EMPLOYEE} , {4 , 1 , 5}},
      { 17, {1,2,3,4,NO_EMPLOYEE}, {25,4,18,3} },
      { 16, {4,4,4,4,NO_EMPLOYEE}, {25,4,18,3} },
      { 17, {4,4,3,4,NO_EMPLOYEE}, {25,4,18,3} },
      { 24, {4,4,3,4,NO_EMPLOYEE,3,3}, {25,4,18,3} } ,
      { 34, {4,4,3,4,NO_EMPLOYEE,3,3,NO_EMPLOYEE,7,7}, {25,4,18,3}},
      { 15, {5,5,5,5,6,10,10,10,10,10,NO_EMPLOYEE}, {1,2,3,4}},
      { 19, {2,2,4,4,NO_EMPLOYEE,1,1,4,3,3}, {1,4,5,3} } ,
      { 34, {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,NO_EMPLOYEE} ,{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28}}
};

#define CHECK(cond, ...)                     \
    do                                       \
    {                                        \
        if (cond)                            \
            break;                           \
        printf("Test failed: " __VA_ARGS__); \
        printf("\n");                        \
        return false;                        \
    } while (0)

bool test(Price p, const std::vector<Employee> &boss, const std::vector<Price> &gp)
{
    auto &&[sol_p, sol_g] = optimize_gifts(boss, gp);
    CHECK(sol_g.size() == boss.size(),
          "Size of the solution: expected %zu but got %zu.", boss.size(), sol_g.size());

    Price real_p = 0;
    for (Gift g : sol_g)
        real_p += gp[g];
    CHECK(real_p == sol_p, "Sum of gift prices is %llu but reported price is %llu.", real_p, sol_p);

    if (0)
    {
        for (Employee e = 0; e < boss.size(); e++)
            printf(" (%zu)%zu", e, sol_g[e]);
        printf("\n");
    }

    for (Employee e = 0; e < boss.size(); e++)
        CHECK(boss[e] == NO_EMPLOYEE || sol_g[boss[e]] != sol_g[e],
              "Employee %zu and their boss %zu has same gift %zu.", e, boss[e], sol_g[e]);

    CHECK(p == sol_p, "Wrong price: expected %llu got %llu.", p, sol_p);

    return true;
}
#undef CHECK

int main()
{
    int ok = 0, fail = 0;
    int i = 1;
    for (auto &&[p, b, gp] : EXAMPLES)
    {
        // (test(p, b, gp) ? ok : fail)++;
        if(test(p,b,gp))
        {
            ok++;
            std::cout << "Test " << i << " SUCCESS" << std::endl;
        }
        else
        {
            fail++;
            std::cout << "Test" << i << " FAIL" << std::endl;
        }
        i++;
        std::cout << "____________________________________________________________________________________________________" << std::endl;

    }

    if (!fail)
        printf("Passed all %d tests!\n", ok);
    else
        printf("Failed %d of %d tests.", fail, fail + ok);
}

#endif
