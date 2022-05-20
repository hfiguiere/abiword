/* AbiSource Program Utilities
 * Copyright (C) 2004-2021 Hubert Figuière
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  
 * 02110-1301 USA.
 */

#import <Cocoa/Cocoa.h>

bool XAP_CocoaAssertMsg(const char* szMsg, const char* szFile, int iLine)
{
    NSString* message = [NSString stringWithFormat:@"ASSERT %s raised at %s:%d", szMsg, szFile, iLine];
    NSLog(@"%@", message);
    bool suppress = false;

    NSAlert* alert = [[NSAlert alloc] init];
    alert.messageText = @"Assertion failed";
    alert.informativeText = message;
    alert.showsSuppressionButton = YES;
    [alert runModal];

    NSButton* button = alert.suppressionButton;
    if (button) {
        suppress = button.state == NSControlStateValueOn;
    }
    [alert release];
    return suppress;
}
