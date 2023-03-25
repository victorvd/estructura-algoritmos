#include <iostream>
#include <unordered_map>
#include <list>

using namespace std;

class LFUCache {
	
	// freqMap : hashmap< freq, list< (key,value)> >
	unordered_map<int, list<pair<int, int>>> freqMap;

	int lowestFreq = 0;
	int cap;

	public:
	// keyMap : unordered_map< key, pair< freq, list< (key,value) >::iterator> >
	unordered_map<int, pair<int, list<pair<int, int>>::iterator>> keyMap;
	
	LFUCache(int capacity) {
		cap = capacity;
	}

	void refer(int key, int value) {
		if (!keyMap.count(key)) {
			if (keyMap.size() < cap) {
				freqMap[1].emplace_back(key, value);
				keyMap[key] = {1, --freqMap[1].end()};
				lowestFreq = 1;
			} else {
				if (lowestFreq == 0) return;
				auto [k, v] = freqMap[lowestFreq].front();
				freqMap[lowestFreq].erase(freqMap[lowestFreq].begin());
				keyMap.erase(k);
				refer(key, value);
			}
		} else {
			auto [freq, it] = keyMap[key];
			if (freqMap[freq].size() == 1 and freq == lowestFreq) lowestFreq++;
			freqMap[freq].erase(it);
			freqMap[freq+1].emplace_back(key, value);
			keyMap[key] = {freq+1, --freqMap[freq+1].end()};
		}
	}
};



void display(const unordered_map<int, pair<int, list<pair<int, int>>::iterator>> &keyMap){

	for (auto const &pair: keyMap) {
		cout << "{" << pair.first << ": " << pair.first << "} ";
	}
	cout<<"\n";
}

// Driver Code
int main()
{
	// Pre defining the size of the cache
	int cachSize = 5;
	LFUCache* myCache = new LFUCache(cachSize);

	// The Numbers to be added in LFU List.
	int arr[] = { 1, 2, 3, 4, 5, 2, 10, 7, 11, 1 };
	//int arr[] = { 2, 3, 2, 1, 5, 2, 4, 5, 3, 2, 5, 2 };
	
	// Number of elements to be added in LFU List.
	int n = sizeof(arr) / sizeof(int);

	for (int i = 0; i < n; ++i) {

		myCache->refer(arr[i],arr[i]);
		
		display(myCache->keyMap);
	}

	return 0;
}
