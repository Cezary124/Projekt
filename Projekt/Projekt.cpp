#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <map>
#include <algorithm>

using namespace std;

struct Params
{
	string inputFilePath;
	string outputFilePath;
};

Params tryGetParams(int argc, char* argv[])
{
	string input, output;
	for (int i = 1; i < argc; ++i)
	{
		if (string(argv[i]) == "-g")
		{
			input = argv[i + 1];
		}
		else if (string(argv[i]) == "-k")
		{
			output = argv[i + 1];
		}
	}
	if (input.length() == 0)
	{
		cerr << "Podaj œcie¿kê do pliku wejœciowego z grafem -g." << endl;
		exit(EXIT_FAILURE);
	}
	if (output.length() == 0)
	{
		cerr << "Podaj œcie¿kê do plik wyjœciowego -k." << endl;
		exit(EXIT_FAILURE);
	}

	return {
		input,
		output,
	};
}

vector<string> getEdgeStrings(string line)
{
	vector<string> edgeStrings;
	istringstream stream(line);

	string edgeString;

	while (getline(stream, edgeString, ')'))
	{
		edgeStrings.push_back(edgeString);
	}

	return edgeStrings;
}

string removeNonNumeric(string inputString)
{
	string result;

	for (char ch : inputString)
	{
		if (isdigit(ch))
		{
			result += ch;
		}
	}

	return result;
}

void readEdgesAndInsertIntoGraph(string line, vector<pair<int, int>>& graph)
{
	vector<string> edgeStrings = getEdgeStrings(line);

	pair<int, int> edge;
	for (const auto& edgeString : edgeStrings)
	{
		string cleanEdgeString = removeNonNumeric(edgeString);

		if (cleanEdgeString.size() == 2)
		{
			edge.first = stoi(cleanEdgeString.substr(0, 1));
			edge.second = stoi(cleanEdgeString.substr(1));

			graph.push_back(edge);
		}
		else if (cleanEdgeString.size() != 0)
		{
			cout << "Niepoprawne dane pliku. Krawêdzie zapisane w formacie par cyfr powinny byæ rozdzielone znakiem ')'. B³¹d w linii: " << line << cleanEdgeString << endl;
			exit(EXIT_FAILURE);
		}
	}
}

vector<pair<int, int>> tryReadGraphFromFile(string path)
{
	ifstream inputFile(path);

	if (!inputFile.is_open())
	{
		cout << "Nie mo¿na otworzyæ pliku: " << path << endl;
		exit(EXIT_FAILURE);
	}

	string line;
	vector<pair<int, int>> graph;
	while (getline(inputFile, line))
	{
		readEdgesAndInsertIntoGraph(line, graph);
	}

	inputFile.close();

	return graph;
}

void displayGraph(vector<pair<int, int>> graph)
{
	for (const auto& edge : graph)
	{
		cout << "(" << edge.first << ", " << edge.second << ")" << endl;
	}
}

set<int> getPoints(vector<pair<int, int>> graph) {
	set<int> uniquePoints;

	for (const auto& edge : graph) {
		uniquePoints.insert(edge.first);
		uniquePoints.insert(edge.second);
	}

	return uniquePoints;
}

bool isValidSolution(map<int, int> coloring, vector<pair<int, int>> graph) {
	for (const auto& edge : graph) {
		if (coloring.at(edge.first) == coloring.at(edge.second)) {
			return false;
		}
	}
	return true;
}

bool findFirstValidSolution(set<int> remainingPoints, map<int, int> currentSolution, int numColors, vector<pair<int, int>> graph, map<int, int>& firstValidSolution) {
	if (remainingPoints.empty()) {
		if (isValidSolution(currentSolution, graph)) {
			firstValidSolution = currentSolution;
			return true;
		}
		return false;
	}

	int currentPoint = *remainingPoints.begin();
	remainingPoints.erase(currentPoint);

	for (int color = 0; color < numColors; ++color) {
		map<int, int> extendedColoring = currentSolution;
		extendedColoring[currentPoint] = color;

		if (findFirstValidSolution(remainingPoints, extendedColoring, numColors, graph, firstValidSolution)) {
			return true;
		}
	}

	return false;
}

bool tryColorPoints(set<int> points, int numColors, vector<pair<int, int>> graph, map<int, int>& coloredPoints) {
	return findFirstValidSolution(points, {}, numColors, graph, coloredPoints);
}

map<int, int> getColoredPoints(vector<pair<int, int>> graph)
{
	set<int> points = getPoints(graph);
	int numberOfColors = 2;
	map<int, int> coloredPoints;
	while (!tryColorPoints(points, numberOfColors, graph, coloredPoints))
	{
		numberOfColors++;
	}

	return coloredPoints;
}

map<int, vector<int>> groupPointsByColor(map<int, int> coloredPoints) {
	map<int, vector<int>> pointsGroupedByColor;

	for (const auto& entry : coloredPoints) {
		int point = entry.first;
		int color = entry.second;

		if (pointsGroupedByColor.find(color) == pointsGroupedByColor.end()) {
			pointsGroupedByColor[color] = { point };
		}
		else {
			pointsGroupedByColor[color].push_back(point);
		}
	}

	return pointsGroupedByColor;
}

void writeToFile(map<int, vector<int>> colorPoints, string path) {

	ofstream outFile(path);

	if (!outFile.is_open())
	{
		cout << "Nie mo¿na otworzyæ pliku: " << path << endl;
		exit(EXIT_FAILURE);
	}

	for (const auto& color : colorPoints) {
		outFile << "Wierzcho³ki o kolorze " << color.first << ": ";

		for (const auto& point : color.second) {
			outFile << point << ", ";
		}

		outFile << endl;
	}

	outFile.close();
}

int main(int argc, char* argv[])
{
	Params params = tryGetParams(argc, argv);
	vector<pair<int, int>> graph = tryReadGraphFromFile(params.inputFilePath);

	displayGraph(graph);

	map<int, int> coloredPoints = getColoredPoints(graph);

	map<int, vector<int>> pointsGroupedByColor = groupPointsByColor(coloredPoints);

	writeToFile(pointsGroupedByColor, params.outputFilePath);

	return 0;
}
