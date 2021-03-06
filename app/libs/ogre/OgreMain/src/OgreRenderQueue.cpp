/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2014 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#include "OgreStableHeaders.h"

#include "OgreRenderQueue.h"

#include "OgreMaterial.h"
#include "OgreRenderQueueSortingGrouping.h"
#include "OgrePass.h"
#include "OgreMaterialManager.h"
#include "OgreSceneManager.h"
#include "OgreMovableObject.h"
#include "OgreSceneManagerEnumerator.h"
#include "OgreTechnique.h"


namespace Ogre {

    //---------------------------------------------------------------------
    RenderQueue::RenderQueue()
        : mRenderableListener(0)
    {
        // Create the 'main' queue up-front since we'll always need that
        mGroups.insert(
            RenderQueueGroupMap::value_type(
                RENDER_QUEUE_MAIN, 
                OGRE_NEW RenderQueueGroup(this) )
            );

        // set default queue
        mDefaultQueueGroup = RENDER_QUEUE_MAIN;
        mDefaultRenderablePriority = OGRE_RENDERABLE_DEFAULT_PRIORITY;

    }
    //---------------------------------------------------------------------
    RenderQueue::~RenderQueue()
    {
        
        // trigger the pending pass updates, otherwise we could leak
        Pass::processPendingPassUpdates();
        
        // Destroy the queues for good
        RenderQueueGroupMap::iterator i, iend;
        i = mGroups.begin();
        iend = mGroups.end();
        for (; i != iend; ++i)
        {
            OGRE_DELETE i->second;
        }
        mGroups.clear();
    }
    //-----------------------------------------------------------------------
    void RenderQueue::addRenderable(Renderable* pRend, uint8 groupID, ushort priority)
    {
        // Find group
        RenderQueueGroup* pGroup = getQueueGroup(groupID);

        Technique* pTech;

        // tell material it's been used
        if (!pRend->getMaterial().isNull())
            pRend->getMaterial()->touch();

        // Check material & technique supplied (the former since the default implementation
        // of getTechnique is based on it for backwards compatibility
        if(pRend->getMaterial().isNull() || !pRend->getTechnique())
        {
            // Use default base white, with lighting only if vertices has normals
            RenderOperation op;
            pRend->getRenderOperation(op);
            bool useLighting = (NULL != op.vertexData->vertexDeclaration->findElementBySemantic(VES_NORMAL));
            MaterialPtr baseWhite = MaterialManager::getSingleton().getByName(useLighting ? "BaseWhite" : "BaseWhiteNoLighting");
            pTech = baseWhite->getTechnique(0);
        }
        else
            pTech = pRend->getTechnique();

        if (mRenderableListener)
        {
            // Allow listener to override technique and to abort
            if (!mRenderableListener->renderableQueued(pRend, groupID, priority, 
                &pTech, this))
                return; // rejected

            // tell material it's been used (incase changed)
            pTech->getParent()->touch();
        }
        
        pGroup->addRenderable(pRend, pTech, priority);

    }
    //-----------------------------------------------------------------------
    void RenderQueue::clear(bool destroyPassMaps)
    {
        // Clear the queues
        SceneManagerEnumerator::SceneManagerIterator scnIt =
            SceneManagerEnumerator::getSingleton().getSceneManagerIterator();

        // Note: We clear dirty passes from all RenderQueues in all 
        // SceneManagers, because the following recalculation of pass hashes
        // also considers all RenderQueues and could become inconsistent, otherwise.
        while (scnIt.hasMoreElements())
        {
            SceneManager* sceneMgr = scnIt.getNext();
            RenderQueue* queue = sceneMgr->getRenderQueue();

            RenderQueueGroupMap::iterator i, iend;
            i = queue->mGroups.begin();
            iend = queue->mGroups.end();
            for (; i != iend; ++i)
            {
                i->second->clear(destroyPassMaps);
            }
        }

        // Now trigger the pending pass updates
        Pass::processPendingPassUpdates();

        // NB this leaves the items present (but empty)
        // We're assuming that frame-by-frame, the same groups are likely to 
        //  be used, so no point destroying the vectors and incurring the overhead
        //  that would cause, let them be destroyed in the destructor.
    }
    //-----------------------------------------------------------------------
    RenderQueue::QueueGroupIterator RenderQueue::_getQueueGroupIterator(void)
    {
        return QueueGroupIterator(mGroups.begin(), mGroups.end());
    }
    //-----------------------------------------------------------------------
    RenderQueue::ConstQueueGroupIterator RenderQueue::_getQueueGroupIterator(void) const
    {
        return ConstQueueGroupIterator(mGroups.begin(), mGroups.end());
    }
    //-----------------------------------------------------------------------
    void RenderQueue::addRenderable(Renderable* pRend, uint8 groupID)
    {
        addRenderable(pRend, groupID, mDefaultRenderablePriority);
    }
    //-----------------------------------------------------------------------
    void RenderQueue::addRenderable(Renderable* pRend)
    {
        addRenderable(pRend, mDefaultQueueGroup, mDefaultRenderablePriority);
    }
    //-----------------------------------------------------------------------
    uint8 RenderQueue::getDefaultQueueGroup(void) const
    {
        return mDefaultQueueGroup;
    }
    //-----------------------------------------------------------------------
    void RenderQueue::setDefaultQueueGroup(uint8 grp)
    {
        mDefaultQueueGroup = grp;
    }
    //-----------------------------------------------------------------------
    ushort RenderQueue::getDefaultRenderablePriority(void) const
    {
        return mDefaultRenderablePriority;
    }
    //-----------------------------------------------------------------------
    void RenderQueue::setDefaultRenderablePriority(ushort priority)
    {
        mDefaultRenderablePriority = priority;
    }
    
    
    //-----------------------------------------------------------------------
    RenderQueueGroup* RenderQueue::getQueueGroup(uint8 groupID)
    {
        // Find group
        RenderQueueGroupMap::iterator groupIt;
        RenderQueueGroup* pGroup;

        groupIt = mGroups.find(groupID);
        if (groupIt == mGroups.end())
        {
            // Insert new
            pGroup = OGRE_NEW RenderQueueGroup(this);
            mGroups.insert(RenderQueueGroupMap::value_type(groupID, pGroup));
        }
        else
        {
            pGroup = groupIt->second;
        }

        return pGroup;

    }
    //-----------------------------------------------------------------------
    void RenderQueue::merge( const RenderQueue* rhs )
    {
        ConstQueueGroupIterator it = rhs->_getQueueGroupIterator( );

        while( it.hasMoreElements() )
        {
            uint8 groupID = it.peekNextKey();
            RenderQueueGroup* pSrcGroup = it.getNext();
            RenderQueueGroup* pDstGroup = getQueueGroup( groupID );

            pDstGroup->merge( pSrcGroup );
        }
    }
}

