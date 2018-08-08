#include "MyTask.h"

const std::shared_ptr<Resource> MyTask::get_resource() { return resource_; };

TaskTarget MyTask::get_target() { return target_; };
void MyTask::set_target(TaskTarget target) { target_ = target; };