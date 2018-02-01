
#include <random>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <fstream>

using namespace std;

#include "Process.h"


/* send message to a set of processes */

void Process::BroadcastMsg(vector<Process>& vp) {
	for (int k = 0; k < number_of_processes; k++) {
		if (k != id) SendMsg(vp[k]);
	}
}

/* send message to a process */

void Process::SendMsg(Process& k)  {
	
	my_time_now = GetTimeNow();
	RefreshClock();

	if (Utility::GetRandomNumberInRange(1, 100)*1.0 <= alpha * 100) {
    // D is distance matrix
		int distance = D[id][k.id];

		int time_delay_msg = 0;
		for (int i = 0; i < distance; i++) {
			time_delay_msg += PhysicalTime::GetDelayMessageTime();
		}
		CountActiveSize();
		int as = CountActiveSizeNow();
		augmented_time.setActiveSize(as);
		k.PushMsg(message(time_delay_msg + absolute_time, augmented_time, id));
	}
}

void Process::CountActiveSize()  {

  // Duong: we only push size of active count into history 
  //        after we have passed the uncertainty period (i.e. epsilon)
	if (absolute_time > epsilon) {
		int count_active = 0;
		for (int i = 0; i < augmented_time.getHVC().size(); i++) {
			if (augmented_time.getHVC()[i] > my_time_now - epsilon) {
				count_active++;
			}
		}
		size_of_at_history.push_back(count_active);
	}
}

int Process::CountActiveSizeNow()  {

	my_time_now = GetTimeNow();
	
	RefreshClock();
	int count_active = 0;
	for (int i = 0; i < augmented_time.getHVC().size(); i++) {
		if (augmented_time.getHVC()[i] > my_time_now - epsilon) {
			count_active++;
		}
	}
	return count_active;
}


void Process::ReceiveMsg(){

	if (!mail_box.empty() && begin(mail_box)->arrival_time <= PhysicalTime::GetAbsoluteTime()) {
		while (!mail_box.empty() && begin(mail_box)->arrival_time <= PhysicalTime::GetAbsoluteTime()) {
			auto m_itr = begin(mail_box);
			vector<int> m_at = m_itr->sender_at.getHVC(); 
			for (int i = 0; i < (int)m_at.size(); i++) {
				augmented_time.setHVCElement(max(m_at[i], augmented_time.getHVC()[i]), i);
			}
			mail_box.erase(m_itr);
		}
		my_time_now = GetTimeNow();
		RefreshClock();
		CountActiveSize();
	}
	
}

void Process::PushMsg(const message& m){
	mail_box.insert(m);
}

int Process::GetTimeNow() const {
  
	if (time_locked) return my_time_now;
	
	return max(my_time_now+1,PhysicalTime::GetTimeNow());

}

// RefreshClock is called whenever process sends or receives messages

void Process::RefreshClock() {

	if (time_locked) return;
	for (int i = 0; i < (int)augmented_time.getHVC().size(); i++) {
		if (i == id) continue;
		if (augmented_time.getHVC()[i] < my_time_now - epsilon ) {
			augmented_time.setHVCElement(my_time_now - epsilon, i);
		}
	}
	
	augmented_time.setHVCElement(my_time_now, id);
	
	time_locked = true;
}




