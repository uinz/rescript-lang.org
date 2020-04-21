/*
 TODO: The way the blog works right now is very rough.
 We don't do any webpack magic to extract content, title preview or frontmatter, and
 don't do any pagination... for now it's not really needed I guess, it would
 still be good to rethink the ways the blog works in a later point of time.

 Docusaurus does a lot of webpack / remark magic in that regard. For my taste,
 it does too much, but here's a link to draw some inspiration from:

 https://github.com/facebook/docusaurus/tree/master/packages/docusaurus-plugin-content-blog/src

 Features like RSS feed etc might be nice, but I guess it's not a core feature
 we need right away.
 */

open Util.ReactStuff;

module Link = Next.Link;

module Badge = {
  [@react.component]
  let make = (~badge: string) => {
    <div
      className="inline-block items-center bg-turtle font-medium tracking-tight text-onyx-80 text-14 px-2 rounded-sm py-1">
      <img className="w-4 inline-block pb-1 mr-1" src="/static/star.svg" />
      badge->s
    </div>;
  };
};
module CategorySelector = {
  [@react.component]
  let make =
      (
        ~categories: array(string),
        ~selected: string,
        ~onSelected: string => unit,
      ) => {
    let active = "bg-snow-dark text-onyx rounded py-1";
    <div
      className="text-16 w-full flex items-center justify-between text-onyx-50">
      {Belt.Array.map(categories, cat => {
         <div
           key=cat
           onClick={evt => {
             evt->ReactEvent.Mouse.preventDefault;
             onSelected(cat);
           }}
           className={
             (
               cat === selected
                 ? active : "hover:cursor-pointer hover:text-onyx"
             )
             ++ "  px-4 inline-block"
           }>
           cat->s
         </div>
       })
       ->ate}
    </div>;
  };
};

module BlogCard = {
  let defaultPreviewImg = "https://res.cloudinary.com/dmm9n7v9f/image/upload/v1587479463/Reason%20Association/reasonml.org/reasonml_art2_1280_vhzxnz.png";
  [@react.component]
  let make =
      (
        ~previewImg: option(string)=?,
        ~title: string="Unknown Title",
        ~author: BlogFrontmatter.Author.t,
        ~category: string,
        ~badge: option(string)=?,
        ~date: Js.Date.t,
        ~slug: string,
      ) => {
    <section className="h-full">
      <div className="relative">
        {switch (badge) {
         | None => React.null
         | Some(badge) =>
           <div className="absolute bottom-0 mb-4 -ml-2">
             <Badge badge />
           </div>
         }}
        <Link href="/blog/[slug]" _as={"/blog/" ++ slug}>
          <a className="h-40 w-auto block mb-4">
            {switch (previewImg) {
             | Some(src) => <img className="mb-4 h-full w-full" src />
             | None =>
               <img
                 className="mb-4 object-cover h-full w-full"
                 src=defaultPreviewImg
               />
             }}
          </a>
        </Link>
      </div>
      <div className="px-2">
        <Link href="/blog/[slug]" _as={"/blog/" ++ slug}>
          <a> <h2 className=Text.H3.default> title->s </h2> </a>
        </Link>
        <div className="text-night-light text-sm">
          category->s
          {j| · |j}->s
          {date->Util.Date.toDayMonthYear->s}
        </div>
      </div>
    </section>;
  };
};

module FeatureCard = {
  [@react.component]
  let make =
      (
        ~previewImg: option(string)=?,
        ~title: string="Unknown Title",
        ~author: BlogFrontmatter.Author.t,
        ~badge: option(string)=?,
        ~date: Js.Date.t,
        ~category: string,
        ~firstParagraph: string="",
        ~slug: string,
      ) => {
    let displayName = BlogFrontmatter.Author.getDisplayName(author);

    let authorImg =
      switch (author.imgUrl->Js.Null.toOption) {
      | Some(src) => <img className="h-full w-full rounded-full" src />
      | None => <NameInitialsAvatar displayName />
      };

    <section
      className="flex sm:px-4 md:px-0 flex-col justify-end lg:flex-row sm:items-center h-full">
      <div
        className="w-full h-full sm:self-start md:self-auto"
        style={Style.make(~maxWidth="38.125rem", ~maxHeight="25.4375rem", ())}>
        <Link href="/blog/[slug]" _as={"/blog/" ++ slug}>
          <a className="relative block">
            {switch (badge) {
             | Some(badge) =>
               <div className="absolute mt-10 ml-4 lg:-ml-4"> <Badge badge /> </div>
             | None => React.null
             }}
            {switch (previewImg) {
             | Some(src) => <img className="h-full w-full" src />
             | None => <div className="bg-night-light" />
             }}
          </a>
        </Link>
      </div>
      <div
        className="relative px-4 lg:self-auto md:pt-12 md:px-20 sm:self-start md:ml-16 md:-mt-20 mt-4 bg-white lg:w-full lg:pt-0 lg:mt-8 lg:px-0 lg:ml-12">
        <div className="max-w-400 ">
          <h2 className=Text.H2.default> title->s </h2>
          <div className="mb-6">
            <div
              className="flex items-center font-medium text-onyx-50 text-sm my-2">
              <div className="inline-block w-4 h-4 mr-2"> authorImg </div>
              <div>
                {switch (author.twitter->Js.Null.toOption) {
                 | Some(handle) =>
                   <a
                     className="hover:text-onyx-80"
                     href={"https://twitter.com/" ++ handle}
                     rel="noopener noreferrer"
                     target="_blank">
                     displayName->s
                   </a>
                 | None => displayName->s
                 }}
                {j| · |j}->s
                category->s
                {j| · |j}->s
                {date->Util.Date.toDayMonthYear->s}
              </div>
            </div>
            <p className="text-night-dark text-16"> firstParagraph->s </p>
          </div>
        </div>
        <Link href="/blog/[slug]" _as={"/blog/" ++ slug}>
          <a> <Button> "Read Article"->s </Button> </a>
        </Link>
      </div>
    </section>;
  };
};

type params = {slug: string};

module Post = {
  type t = {
    id: string,
    frontmatter: BlogFrontmatter.t,
  };

  let orderByDate = (posts: array(t)): array(t) => {
    posts
    ->Js.Array.copy
    ->Js.Array2.sortInPlaceWith((a, b) => {
        let aV = a.frontmatter.date->DateStr.toDate->Js.Date.valueOf;
        let bV = b.frontmatter.date->DateStr.toDate->Js.Date.valueOf;
        if (aV === bV) {
          0;
        } else if (aV > bV) {
          (-1);
        } else {
          1;
        };
      });
  };
};

module Malformed = {
  type t = {
    id: string,
    message: string,
  };
};

type props = {
  posts: array(Post.t),
  malformed: array(Malformed.t),
};

let default = (props: props): React.element => {
  let {posts, malformed} = props;

  let (currentCategory, setCategory) = React.useState(() => "All");

  let errorBox =
    if (ProcessEnv.env === ProcessEnv.development
        && Belt.Array.length(malformed) > 0) {
      <div className="mb-12">
        <Markdown.Warn>
          <h2 className="font-bold text-night-dark text-2xl mb-2">
            "Some Blog Posts are Malformed!"->s
          </h2>
          <p>
            "Any blog post with invalid data will not be displayed in production."
            ->s
          </p>
          <div>
            <p className="font-bold mt-4"> "Errors:"->s </p>
            <ul>
              {Belt.Array.map(malformed, m => {
                 <li className="list-disc ml-5">
                   {("pages/blog/" ++ m.id ++ ".mdx: " ++ m.message)->s}
                 </li>
               })
               ->ate}
            </ul>
          </div>
        </Markdown.Warn>
      </div>;
    } else {
      React.null;
    };

  let content =
    if (Belt.Array.length(posts) === 0) {
      <div> "Currently no posts available"->s </div>;
    } else {
      let first = Belt.Array.getExn(posts, 0);
      let rest = Js.Array2.sliceFrom(posts, 1);

      let categories = [|
        "All",
        "Syntax",
        "Compiler",
        "Ecosystem",
        "Docs",
        "Community",
      |];

      <>
        /* We hide the Category Selector for mobile for now*/
        <div className="hidden sm:flex justify-center ">
          <div
            className="my-16 w-full"
            style={Style.make(~maxWidth="32rem", ())}>
            <CategorySelector
              categories
              onSelected={category => setCategory(_ => category)}
              selected=currentCategory
            />
          </div>
        </div>
        <div className="mb-24">
          <FeatureCard
            previewImg=?{first.frontmatter.previewImg->Js.Null.toOption}
            title={first.frontmatter.title}
            badge=?{
              first.frontmatter.badge
              ->Js.Null.toOption
              ->Belt.Option.map(BlogFrontmatter.Badge.toString)
            }
            author={first.frontmatter.author}
            firstParagraph=?{first.frontmatter.description->Js.Null.toOption}
            date={first.frontmatter.date->DateStr.toDate}
            category={
              first.frontmatter.category->BlogFrontmatter.Category.toString
            }
            slug={first.id}
          />
        </div>
        <div
          className="mx-4 xl:mx-0 grid grid-cols-1 xs:grid-cols-3 gap-20 row-gap-12 md:row-gap-24 w-full">
          {Belt.Array.mapWithIndex(
             rest,
             (i, post) => {
               let badge =
                 post.frontmatter.badge
                 ->Js.Null.toOption
                 ->Belt.Option.map(BlogFrontmatter.Badge.toString);
               <BlogCard
                 key={post.id ++ Belt.Int.toString(i)}
                 previewImg=?{post.frontmatter.previewImg->Js.Null.toOption}
                 title={post.frontmatter.title}
                 author={post.frontmatter.author}
                 ?badge
                 category={
                   post.frontmatter.category->BlogFrontmatter.Category.toString
                 }
                 date={post.frontmatter.date->DateStr.toDate}
                 slug={post.id}
               />;
             },
           )
           ->ate}
        </div>
      </>;
    };

  let overlayState = React.useState(() => false);

  <>
    <Meta />
    <div className="mb-32 mt-16 pt-2">
      <div className="text-night text-lg">
        <Navigation overlayState />
        <div className="flex justify-center overflow-hidden">
          <main
            className="min-w-320 lg:align-center w-full lg:px-0 max-w-1280 " /*++ (isOpen ? " hidden" : "")*/>
            <Mdx.Provider components=Markdown.default>
              <div className="flex justify-center">
                <div style={Style.make(~maxWidth="66.625rem", ())}>
                  errorBox
                  content
                </div>
              </div>
            </Mdx.Provider>
          </main>
        </div>
      </div>
    </div>
  </>;
};

let getStaticProps: Next.GetStaticProps.t(props, params) =
  _ctx => {
    let authors = BlogFrontmatter.Author.getAllAuthors();
    let (posts, malformed) =
      BlogApi.getAllPosts()
      ->Belt.Array.reduce(
          ([||], [||]),
          (acc, postData) => {
            let (posts, malformed) = acc;
            let id = postData.slug;

            let decoded =
              BlogFrontmatter.decode(~authors, postData.frontmatter);

            switch (decoded) {
            | Error(message) =>
              let m = {Malformed.id, message};
              let malformed = Belt.Array.concat(malformed, [|m|]);
              (posts, malformed);
            | Ok(frontmatter) =>
              let p = {Post.id, frontmatter};
              let posts = Belt.Array.concat(posts, [|p|]);
              (posts, malformed);
            };
          },
        );

    let props = {posts: Post.orderByDate(posts), malformed};

    Promise.resolved({"props": props});
  };
