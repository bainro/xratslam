/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 * A set of VisualTemplate objects.
 *
 */

#ifndef _xratslam_VisualTemplateSet_
#define _xratslam_VisualTemplateSet_


#include <vector>
#include <string>


/*************************************************************************/
/*************************** VisualTemplateSet ***************************/

namespace xratslam
{
  class VisualTemplate;

  
  /*********************/
  class VisualTemplateSet
  {
    typedef std::vector<VisualTemplate*> Set;

    
  public:

    // Iterates backward.
    typedef Set::reverse_iterator iterator;
    
    
    /** Starts with no elements.
     */
    VisualTemplateSet( int reserveSize =1000 );
    
    ~VisualTemplateSet();

    bool empty()  { return _set.empty(); }
    int  size()   { return _set.size();  }

    /** Iterates backwards.
     */
    iterator begin();

    /** Iterates backwards.
     */
    iterator end()  { return _set.rend(); }
    
    /** Always inserts at the end.
     */
    void add( VisualTemplate *vt );

    /** Save template set.
     * Obs: only write templates with ID > 'lastSavedId'.
     * Return the ID of last saved template.
     */
    int save( std::string dir, int lastSavedId );
    
    /** Load template set from given 'dir'.
     * If '_maxVtId' is already in the memory, all templates with
     * ID greater than '_maxVtId' are removed.
     * Return the ID of last template.
     */
    int load( std::string dir, int maxVtId );


  private:

    void clear();

    
    Set _set;
    int _maxVtId;  // Only VTs wiht ID <= are used.
  };
}

#endif
