/* -*- c-basic-offset: 8 -*-
   FreeRDP: A Remote Desktop Protocol client.
   Redirected Device Manager

   Copyright (C) Marc-Andre Moreau <marcandre.moreau@gmail.com> 2010

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef __DEVMAN_H
#define __DEVMAN_H

#include <freerdp/types_ui.h>

typedef struct _SERVICE SERVICE;
typedef struct _DEVICE DEVICE;

struct _SERVICE
{
	uint32 type;
	int(*create) (DEVICE * dev);
	int(*close) (DEVICE * dev);
	int(*read) (DEVICE * dev);
	int(*write) (DEVICE * dev);
	int(*control) (DEVICE * dev);
	int(*free) (DEVICE * dev);
};
typedef SERVICE * PSERVICE;

struct _DEVICE
{
	uint32 id;
	char* name;
	void* info;
	void* prev;
	void* next;
	SERVICE* service;
};
typedef DEVICE * PDEVICE;

struct _DEVMAN
{
	int count; /* device count */
	int id_sequence; /* generate unique device id */
	DEVICE* idev; /* iterator device */
	DEVICE* head; /* head device in linked list */
	DEVICE* tail; /* tail device in linked list */
	void* pDevmanEntryPoints; /* entry points for device services */
};
typedef struct _DEVMAN DEVMAN;
typedef DEVMAN * PDEVMAN;

typedef PSERVICE (*PDEVMAN_REGISTER_SERVICE)(PDEVMAN devman);
typedef int (*PDEVMAN_UNREGISTER_SERVICE)(PDEVMAN devman, PSERVICE srv);
typedef PDEVICE (*PDEVMAN_REGISTER_DEVICE)(PDEVMAN devman, PSERVICE srv, char* name);
typedef int (*PDEVMAN_UNREGISTER_DEVICE)(PDEVMAN devman, PDEVICE dev);

struct _DEVMAN_ENTRY_POINTS
{
    PDEVMAN_REGISTER_SERVICE pDevmanRegisterService;
    PDEVMAN_UNREGISTER_SERVICE pDevmanUnregisterService;
    PDEVMAN_REGISTER_DEVICE pDevmanRegisterDevice;
    PDEVMAN_UNREGISTER_DEVICE pDevmanUnregisterDevice;
    void* pExtendedData; /* extended data field to pass initial parameters */
};
typedef struct _DEVMAN_ENTRY_POINTS DEVMAN_ENTRY_POINTS;
typedef DEVMAN_ENTRY_POINTS * PDEVMAN_ENTRY_POINTS;

typedef int (*PDEVICE_SERVICE_ENTRY)(PDEVMAN, PDEVMAN_ENTRY_POINTS);

DEVMAN*
devman_new(void* data);
int
devman_free(DEVMAN* devman);
SERVICE*
devman_register_service(DEVMAN* devman);
int
devman_unregister_service(DEVMAN* devman, SERVICE* srv);
DEVICE*
devman_register_device(DEVMAN* devman, SERVICE* srv, char* name);
int
devman_unregister_device(DEVMAN* devman, DEVICE* dev);
void
devman_rewind(DEVMAN* devman);
int
devman_has_next(DEVMAN* devman);
DEVICE*
devman_get_next(DEVMAN* devman);
DEVICE*
devman_get_device_by_id(DEVMAN* devman, uint32 id);
int
devman_load_device_service(DEVMAN* devman, char* filename);

#endif // __DEVMAN_H

