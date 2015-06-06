#include "gamemodel.h"

GameModel::GameModel(QObject *parent) : QObject(parent)
{
    // TODO: how localization?

    // TODO: using test data
    QVariantMap details;

    details["id"] = "tictactoe-0.5";
    details["name"] = "TicTacToe";
    details["description"] = "Game of 'x' and 'o'. Try to get three in a row!";
    _games["tictactoe"] = details;

    details["id"] = "react";
    details["name"] = "React!";
    details["description"] = "Who enters fastest sequence of numbers?";
    _games["react"] = details;

    details["id"] = "testgame";
    details["name"] = "Test Game";
    details["description"] = "Just a test game.";
    _games["testgame"] = details;
}

GameModel::~GameModel()
{

}

QStringList GameModel::localGameIds() const
{
    return _games.keys();
}


QVariantMap GameModel::game(QString gameId) const
{
    if (_games.contains(gameId)) {
        return _games[gameId];
    } else {
        // not found, return empty
        QVariantMap empty;
        return empty;
    }
}
