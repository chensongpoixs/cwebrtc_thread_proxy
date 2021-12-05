/********************************************************************
created:	2021-11-24

author:		chensong

purpose:	cproxy

*********************************************************************/

#include "cproxy.h"

namespace chen {
	namespace internal {

		SynchronousMethodCall::SynchronousMethodCall( MessageHandler* proxy)
			: proxy_(proxy) {}

		SynchronousMethodCall::~SynchronousMethodCall() = default;

		void SynchronousMethodCall::Invoke(const Location& posted_from ,  Thread* t ) {
			//TODO@chensong 2021-11-22 event --- ^_^
			if (t->IsCurrent()) {
				proxy_->OnMessage(nullptr);
			} 
			else 
			{ 
				t->Post(posted_from, this, 0);
				e_.Wait(Event::kForever);
			}
		}

		void SynchronousMethodCall::OnMessage(Message*) 
		{
			proxy_->OnMessage(nullptr);
			e_.Set();
		}

	}  // namespace internal
}  // namespace chen

