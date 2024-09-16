#include "ElimateTest.h"



void ElimateTest::horizonTest1()
{
    Game game;
    int matrix[MAX_M+2][MAX_N+2]={
        {0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 1, 2, 0, 2, 1, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 1, 2, 1, 2, 1, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0}
    };

    game.setBoxMatrix(matrix, 5, 5);
    QVERIFY(game.elimatable(2, 2, 2, 4)==true);
}

void ElimateTest::horizonTest2()
{
    Game game;
    int matrix[MAX_M+2][MAX_N+2]={
        {0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 1, 2, 0, 2, 1, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 1, 2, 1, 2, 1, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0}
    };

    game.setBoxMatrix(matrix, 5, 5);
    QVERIFY(game.elimatable(4, 2, 4, 4)==false);
}

void ElimateTest::verticalTest1()
{
    Game game;
    int matrix[MAX_M+2][MAX_N+2]={
        {0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 1, 2, 1, 2, 1, 0},
        {0, 1, 0, 1, 1, 1, 0},
        {0, 1, 2, 1, 2, 1, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0}
    };

    game.setBoxMatrix(matrix, 5, 5);
    QVERIFY(game.elimatable(2, 2, 4, 2)==true);
}

void ElimateTest::verticalTest2()
{
    Game game;
    int matrix[MAX_M+2][MAX_N+2]={
        {0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 1, 2, 1, 2, 1, 0},
        {0, 1, 0, 1, 1, 1, 0},
        {0, 1, 2, 1, 2, 1, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0}
    };

    game.setBoxMatrix(matrix, 5, 5);
    QVERIFY(game.elimatable(2, 4, 4, 4)==false);
}

void ElimateTest::oneCornerTest1()
{ 
    Game game;
    int matrix[MAX_M+2][MAX_N+2]={
        {0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 1, 2, 1, 1, 1, 0},
        {0, 1, 0, 1, 1, 1, 0},
        {0, 1, 0, 0, 2, 1, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0}
    };

    game.setBoxMatrix(matrix, 5, 5);
    QVERIFY(game.elimatable(4, 4, 2, 2)==true);
}

void ElimateTest::oneCornerTest2()
{ 
    Game game;
    int matrix[MAX_M+2][MAX_N+2]={
        {0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 1, 2, 1, 1, 1, 0},
        {0, 1, 0, 1, 1, 1, 0},
        {0, 1, 0, 0, 2, 1, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0}
    };

    game.setBoxMatrix(matrix, 5, 5);
    QVERIFY(game.elimatable(2, 2, 4, 4)==true);
}

void ElimateTest::oneCornerTest3()
{ 
    Game game;
    int matrix[MAX_M+2][MAX_N+2]={
        {0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 1, 2, 0, 0, 1, 0},
        {0, 1, 1, 1, 0, 1, 0},
        {0, 1, 1, 1, 2, 1, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0}
    };

    game.setBoxMatrix(matrix, 5, 5);
    QVERIFY(game.elimatable(4, 4, 2, 2)==true);
}

void ElimateTest::oneCornerTest4()
{ 
    Game game;
    int matrix[MAX_M+2][MAX_N+2]={
        {0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 1, 2, 0, 0, 1, 0},
        {0, 1, 1, 1, 0, 1, 0},
        {0, 1, 1, 1, 2, 1, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0}
    };

    game.setBoxMatrix(matrix, 5, 5);
    QVERIFY(game.elimatable(2, 2, 4, 4)==true);
}   

void ElimateTest::oneCornerTest5()
{ 
    Game game;
    int matrix[MAX_M+2][MAX_N+2]={
        {0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 1, 0, 0, 2, 1, 0},
        {0, 1, 0, 1, 1, 1, 0},
        {0, 1, 2, 1, 1, 1, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0}
    };

    game.setBoxMatrix(matrix, 5, 5);
    QVERIFY(game.elimatable(2, 4, 4, 2)==true);
}

void ElimateTest::oneCornerTest6()
{ 
    Game game;
    int matrix[MAX_M+2][MAX_N+2]={
        {0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 1, 0, 0, 2, 1, 0},
        {0, 1, 0, 1, 1, 1, 0},
        {0, 1, 2, 1, 1, 1, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0}
    };

    game.setBoxMatrix(matrix, 5, 5);
    QVERIFY(game.elimatable(4, 2, 2, 4)==true);
}

void ElimateTest::oneCornerTest7()
{ 
    Game game;
    int matrix[MAX_M+2][MAX_N+2]={
        {0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 1, 1, 1, 2, 1, 0},
        {0, 1, 1, 1, 0, 1, 0},
        {0, 1, 2, 0, 0, 1, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0}
    };

    game.setBoxMatrix(matrix, 5, 5);
    QVERIFY(game.elimatable(2, 4, 4, 2)==true);
}

void ElimateTest::oneCornerTest8()
{ 
    Game game;
    int matrix[MAX_M+2][MAX_N+2]={
        {0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 1, 1, 1, 2, 1, 0},
        {0, 1, 1, 1, 0, 1, 0},
        {0, 1, 2, 0, 0, 1, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0}
    };

    game.setBoxMatrix(matrix, 5, 5);
    QVERIFY(game.elimatable(4, 2, 2, 4)==true);
}

void ElimateTest::oneCornerTest9()
{
    Game game;
    int matrix[MAX_M+2][MAX_N+2]={
        {0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 1, 2, 1, 1, 1, 0},
        {0, 1, 1, 2, 1, 1, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0}
    };

    game.setBoxMatrix(matrix, 5, 5);
    QVERIFY(game.elimatable(2, 2, 3, 3)==false);
}

void ElimateTest::oneCornerTest10()
{
    Game game;
    int matrix[MAX_M+2][MAX_N+2]={
        {0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 1, 2, 1, 1, 1, 0},
        {0, 1, 1, 2, 1, 1, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0}
    };

    game.setBoxMatrix(matrix, 5, 5);
    QVERIFY(game.elimatable(3, 3, 2, 2)==false);
}

void ElimateTest::oneCornerTest11()
{
    Game game;
    int matrix[MAX_M+2][MAX_N+2]={
        {0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 1, 1, 2, 1, 1, 0},
        {0, 1, 2, 1, 1, 1, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0}
    };

    game.setBoxMatrix(matrix, 5, 5);
    QVERIFY(game.elimatable(2, 3, 3, 2)==false);
}

void ElimateTest::oneCornerTest12()
{
    Game game;
    int matrix[MAX_M+2][MAX_N+2]={
        {0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 1, 1, 2, 1, 1, 0},
        {0, 1, 2, 1, 1, 1, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0}
    };

    game.setBoxMatrix(matrix, 5, 5);
    QVERIFY(game.elimatable(3, 2, 2, 3)==false);
}

void ElimateTest::twoCornerTest1()
{
    Game game;
    int matrix[MAX_M+2][MAX_N+2]={
        {0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 1, 1, 2, 0, 2, 0},
        {0, 1, 1, 1, 0, 1, 0},
        {0, 1, 2, 0, 0, 1, 0},
        {0, 1, 1, 1, 0, 1, 0},
        {0, 1, 1, 2, 0, 2, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0}
    };

    game.setBoxMatrix(matrix, 7, 5);
    QVERIFY(game.elimatable(4, 2, 2, 3)==true);
}

void ElimateTest::twoCornerTest2()
{
    Game game;
    int matrix[MAX_M+2][MAX_N+2]={
        {0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 1, 1, 2, 0, 2, 0},
        {0, 1, 1, 1, 0, 1, 0},
        {0, 1, 2, 0, 0, 1, 0},
        {0, 1, 1, 1, 0, 1, 0},
        {0, 1, 1, 2, 0, 2, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0}
    };

    game.setBoxMatrix(matrix, 7, 5);
    QVERIFY(game.elimatable(4, 2, 2, 5)==true);
}

void ElimateTest::twoCornerTest3()
{
    Game game;
    int matrix[MAX_M+2][MAX_N+2]={
        {0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 1, 1, 2, 0, 2, 0},
        {0, 1, 1, 1, 0, 1, 0},
        {0, 1, 2, 0, 0, 1, 0},
        {0, 1, 1, 1, 0, 1, 0},
        {0, 1, 1, 2, 0, 2, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0}
    };

    game.setBoxMatrix(matrix, 7, 5);
    QVERIFY(game.elimatable(4, 2, 6, 3)==true);
}

void ElimateTest::twoCornerTest4()
{
    Game game;
    int matrix[MAX_M+2][MAX_N+2]={
        {0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 1, 1, 2, 0, 2, 0},
        {0, 1, 1, 1, 0, 1, 0},
        {0, 1, 2, 0, 0, 1, 0},
        {0, 1, 1, 1, 0, 1, 0},
        {0, 1, 1, 2, 0, 2, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0}
    };

    game.setBoxMatrix(matrix, 7, 5);
    QVERIFY(game.elimatable(4, 2, 6, 5)==true);
}

void ElimateTest::twoCornerTest5()
{
    Game game;
    int matrix[MAX_M+2][MAX_N+2]={
        {0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 2, 0, 2, 1, 1, 0},
        {0, 1, 0, 1, 1, 1, 0},
        {0, 1, 0, 0, 2, 1, 0},
        {0, 1, 0, 1, 1, 1, 0},
        {0, 2, 0, 2, 1, 1, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0}
    };

    game.setBoxMatrix(matrix, 7, 5);
    QVERIFY(game.elimatable(4, 4, 2, 1)==true);
}

void ElimateTest::twoCornerTest6()
{
    Game game;
    int matrix[MAX_M+2][MAX_N+2]={
        {0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 2, 0, 2, 1, 1, 0},
        {0, 1, 0, 1, 1, 1, 0},
        {0, 1, 0, 0, 2, 1, 0},
        {0, 1, 0, 1, 1, 1, 0},
        {0, 2, 0, 2, 1, 1, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0}
    };

    game.setBoxMatrix(matrix, 7, 5);
    QVERIFY(game.elimatable(4, 4, 6, 1)==true);
}

void ElimateTest::twoCornerTest7()
{
    Game game;
    int matrix[MAX_M+2][MAX_N+2]={
        {0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 2, 0, 2, 1, 1, 0},
        {0, 1, 0, 1, 1, 1, 0},
        {0, 1, 0, 0, 2, 1, 0},
        {0, 1, 0, 1, 1, 1, 0},
        {0, 2, 0, 2, 1, 1, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0}
    };

    game.setBoxMatrix(matrix, 7, 5);
    QVERIFY(game.elimatable(4, 4, 6, 3)==true);
}

void ElimateTest::twoCornerTest8()
{
    Game game;
    int matrix[MAX_M+2][MAX_N+2]={
        {0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 2, 0, 2, 1, 1, 0},
        {0, 1, 0, 1, 1, 1, 0},
        {0, 1, 0, 0, 2, 1, 0},
        {0, 1, 0, 1, 1, 1, 0},
        {0, 2, 0, 2, 1, 1, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0}
    };

    game.setBoxMatrix(matrix, 7, 5);
    QVERIFY(game.elimatable(4, 4, 2, 3)==true);
}

void ElimateTest::twoCornerTest9()
{
    Game game;
    int matrix[MAX_M+2][MAX_N+2]={
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 0},
        {0, 1, 1, 1, 2, 1, 1, 1, 0},
        {0, 1, 2, 1, 0, 1, 2, 1, 0},
        {0, 1, 0, 0, 0, 0, 0, 1, 0},
        {0, 1, 2, 1, 1, 1, 2, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
    };

    game.setBoxMatrix(matrix, 5, 7);
    QVERIFY(game.elimatable(2, 4, 3, 2)==true);
}

void ElimateTest::twoCornerTest10()
{
    Game game;
    int matrix[MAX_M+2][MAX_N+2]={
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 0},
        {0, 1, 1, 1, 2, 1, 1, 1, 0},
        {0, 1, 2, 1, 0, 1, 2, 1, 0},
        {0, 1, 0, 0, 0, 0, 0, 1, 0},
        {0, 1, 2, 1, 1, 1, 2, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
    };

    game.setBoxMatrix(matrix, 5, 7);
    QVERIFY(game.elimatable(2, 4, 5, 2)==true);
}

void ElimateTest::twoCornerTest11()
{
    Game game;
    int matrix[MAX_M+2][MAX_N+2]={
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 0},
        {0, 1, 1, 1, 2, 1, 1, 1, 0},
        {0, 1, 2, 1, 0, 1, 2, 1, 0},
        {0, 1, 0, 0, 0, 0, 0, 1, 0},
        {0, 1, 2, 1, 1, 1, 2, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
    };

    game.setBoxMatrix(matrix, 5, 7);
    QVERIFY(game.elimatable(2, 4, 3, 6)==true);
}

void ElimateTest::twoCornerTest12()
{
    Game game;
    int matrix[MAX_M+2][MAX_N+2]={
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 0},
        {0, 1, 1, 1, 2, 1, 1, 1, 0},
        {0, 1, 2, 1, 0, 1, 2, 1, 0},
        {0, 1, 0, 0, 0, 0, 0, 1, 0},
        {0, 1, 2, 1, 1, 1, 2, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
    };

    game.setBoxMatrix(matrix, 5, 7);
    QVERIFY(game.elimatable(2, 4, 5, 6)==true);
}

void ElimateTest::twoCornerTest13()
{
    Game game;
    int matrix[MAX_M+2][MAX_N+2]={
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 2, 1, 1, 1, 2, 1, 0},
        {0, 1, 0, 0, 0, 0, 0, 1, 0},
        {0, 1, 2, 1, 0, 1, 2, 1, 0},
        {0, 1, 1, 1, 2, 1, 1, 1, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
    };

    game.setBoxMatrix(matrix, 5, 7);
    QVERIFY(game.elimatable(4, 4, 1, 2)==true);
}

void ElimateTest::twoCornerTest14()
{
    Game game;
    int matrix[MAX_M+2][MAX_N+2]={
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 2, 1, 1, 1, 2, 1, 0},
        {0, 1, 0, 0, 0, 0, 0, 1, 0},
        {0, 1, 2, 1, 0, 1, 2, 1, 0},
        {0, 1, 1, 1, 2, 1, 1, 1, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
    };

    game.setBoxMatrix(matrix, 5, 7);
    QVERIFY(game.elimatable(4, 4, 3, 2)==true);
}

void ElimateTest::twoCornerTest15()
{
    Game game;
    int matrix[MAX_M+2][MAX_N+2]={
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 2, 1, 1, 1, 2, 1, 0},
        {0, 1, 0, 0, 0, 0, 0, 1, 0},
        {0, 1, 2, 1, 0, 1, 2, 1, 0},
        {0, 1, 1, 1, 2, 1, 1, 1, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
    };

    game.setBoxMatrix(matrix, 5, 7);
    QVERIFY(game.elimatable(4, 4, 1, 6)==true);
}

void ElimateTest::twoCornerTest16()
{
    Game game;
    int matrix[MAX_M+2][MAX_N+2]={
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 2, 1, 1, 1, 2, 1, 0},
        {0, 1, 0, 0, 0, 0, 0, 1, 0},
        {0, 1, 2, 1, 0, 1, 2, 1, 0},
        {0, 1, 1, 1, 2, 1, 1, 1, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
    };

    game.setBoxMatrix(matrix, 5, 7);
    QVERIFY(game.elimatable(4, 4, 3, 6)==true);
}
