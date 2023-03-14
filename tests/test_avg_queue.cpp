#include "utils/avg_queue.h"
#include "rmdecis/core.h"

int main() {
	
	auto queue = AvgQueue<float>(10);
	for (int i = 90; i <= 100; i++) {
		queue.push(i);
		info("Sum: {}, avg: {}", queue.sum_, queue.getAvg());
	}
}