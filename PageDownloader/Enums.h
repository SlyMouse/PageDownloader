/**
 * @brief Enums that are used in program
 * 
 * @file Enums.h
 * @author Artyom Pashkin
 * @date 09.08.2018
 */

#ifndef ENUMS_H
#define ENUMS_H

/**
 * @brief Task's current target
 */
enum class TaskTarget { 
    DownloadAndParse,   //!< Resource must be downloaded and parsed 
    Replace,            //!< Resource's child resources must be replaced inside content
    Save,               //!< Resource must be saved to the disk
    Done                //!< Resource is handled
    };

/**
 * @brief Resource type
 */
enum class ResourceType { 
    Page,   //!< Resource is html page
    Css,    //!< Resource is CSS file
    Other   //!< Resource is other type of file
    }; 
#endif