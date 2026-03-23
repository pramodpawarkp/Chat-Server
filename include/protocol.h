#ifndef PROTOCOL_H
#define PROTOCOL_H

#define CMD_LOGIN 1
#define CMD_MSG 2
#define CMD_PMSG 3
#define CMD_LIST 4
#define CMD_UNKNOWN 0

int parse_message(char* input, char* command, char* data);

#endif