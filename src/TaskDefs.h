/*
 * Copyright 2016 Paradoxon <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef TASK_DEFS_H
#define TASK_DEFS_H

#define	APP_SIG	"application/x-vnd.Tasks"

#define TASK_DIRECTORY "tasks"

#define TASK_MIMETYPE "application/x-tasks"

static const uint32 LOAD_TASKS				= 'load';
static const uint32 LOAD_TASKS_ERROR		= 'lERR';
static const uint32 ADD_TASK				= 'addT';
static const uint32 ADD_TASK_ERROR			= 'aERR';
static const uint32 ADD_TASK_LIST			= 'addL';
static const uint32 ADD_TASK_LIST_ERROR		= 'alER';
static const uint32 REMOVE_TASK				= 'rmTs';
static const uint32 REMOVE_TASK_ERROR		= 'rERR';
static const uint32 REMOVE_TASK_LIST		= 'rmTl';
static const uint32 REMOVE_TASK_LIST_ERROR	= 'rlER';
static const uint32 MODIFY_TASK				= 'mTsk';
static const uint32 MODIFY_TASK_ERROR		= 'mERR';
static const uint32 ADD_TASK_SYNC			= 'addS';
static const uint32 ADD_TASK_SYNC_ERROR		= 'asER';
static const uint32 REMOVE_TASK_SYNC		= 'rmSy';
static const uint32 REMOVE_TASK_SYNC_ERROR	= 'rsER';
static const uint32 MERGE_TASKS				= 'TmT';
static const uint32 MERGE_TASKS_ERROR		= 'TmER';



#endif // _H
