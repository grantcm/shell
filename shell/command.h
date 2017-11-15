//
//  command.h
//  shell
//
//  Created by Grant Miller on 9/8/17.
//  Copyright © 2017 Grant Miller. All rights reserved.
//

#ifndef command_h
#define command_h

struct Command {
    struct Command * next;
    int argc;
    char * env;
    char ** args;
};



#endif /* command_h */
