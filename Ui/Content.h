#ifndef __CONTENT_H__
#define __CONTENT_H__

#include <cppcms/view.h>
#include <cppcms/form.h>

namespace content {
	struct searchForm : public cppcms::form {
		cppcms::widgets::text key;
		cppcms::widgets::submit submit;
		cppcms::widgets::submit prev_page;
		cppcms::widgets::submit next_page;
		searchForm() {
			key.id("input-holder");
			key.non_empty();
			submit.id("search");
			submit.name("submit");
			submit.value("GO");
			prev_page.id("prev_page");
			prev_page.name("submit");
			prev_page.value("previous page");
			next_page.id("next_page");
			next_page.name("submit");
			next_page.value("next page");
			add(key);
			add(submit);
			add(prev_page);
			add(next_page);
		}
		virtual bool validate() {
			return true;
		}
	};
	struct searchMessage : public cppcms::base_content {
		searchForm info;
		std::string key;
		std::string valid;
		std::string result_info;
		std::string result_0_title;
		std::string result_0_abstr;
		std::string result_1_title;
		std::string result_1_abstr;
		std::string result_2_title;
		std::string result_2_abstr;
		std::string result_3_title;
		std::string result_3_abstr;
		std::string result_4_title;
		std::string result_4_abstr;
		std::string result_5_title;
		std::string result_5_abstr;
		std::string result_6_title;
		std::string result_6_abstr;
		std::string result_7_title;
		std::string result_7_abstr;
		std::string result_8_title;
		std::string result_8_abstr;
		std::string result_9_title;
		std::string result_9_abstr;
		std::string if_prev_page;
		std::string if_next_page;
		searchMessage() {
			valid = "";
			result_info = "I'm feeling lucky!";
			if_prev_page = "";
			if_next_page = "";
		}
	};
}

#endif
